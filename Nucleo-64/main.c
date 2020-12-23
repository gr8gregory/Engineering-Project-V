/***************************************************************************
 * main.c
 *
 * Mainline function to initialize Nucleo64 MCB Project and then parse input
 * 	and trigger ISRs to deal with input commands and hardware
 *
 * Caleb Hoeksema, Gregory Huras, Andrew Sammut
 * October 2020
 **************************************************************************/

/*
 * MAYBE ADD A "KILL" COMMAND TO STOP EVERYTHING IF NECESSARY
 */


// Header files
#include "virtualPort.h"
#include "USART.h"
#include "LED.h"
#include "Heartbeat.h"
#include "stepper.h"
#include "limit.h"
#include "servo.h"
#include "dcmotor.h"


#define SERIAL_OPTION (vportInput[1])	
#define COMMAND_ENTERED vportInput[vportOffset] == '\r'
#define SERVO_CMD ('T')
#define STEPPER_CMD ('S')
#define PING_CMD ('P')
#define FLIP_CMD ('F')
#define DC_CMD ('D')

// Global variables
extern volatile uint8_t rxFlag;													// Indicates serial input (command input)
extern volatile uint8_t vportInput[MAX_SER_BUF_SIZE];		// Buffer containing command
extern volatile uint32_t vportOffset;										// Offset of current command
extern volatile uint8_t pingFlag;												// Indicates ping request
extern volatile uint8_t ledFlag;												// Indicates change LED state
extern volatile uint32_t stepGoToPos_step;							// Inidicates the position to send the stepper to
extern uint32_t stepCount_step;
extern volatile uint8_t dirL;
extern volatile uint8_t dirR;


// Function Prototypes (should probably be in some kind of header file)
uint8_t prompt(void);
void clearInput(void);
void betterMain(void);

//A protype of main that does not suck


int main(void)
{
	// Initialization
	System_Clock_Init();			// Set up clock
	SystemCoreClockUpdate();	// Set up clock
	virtualPortInit();				// Set up the terminal (Currently on an ISR)
	limit_Init();							// Set up limit switches
	stepper_Init();						// Set up stepper motor
	servo_Init();
	DC_DIR_Init();						// Set up DC motor directions (do before drive)
	DC_DRV_Init();						// Set up DC motor drive/speed
	Heartbeat_Init();					// Set up the heartbeat timer (Currently reads any input-related ISRs)
	
	DisableInterrupts; 				// Don't want to catch anything from the heartbeat yet
	LED_Init();									// Set up LED flashing (if we need it)
	
	vportMenuPrint();							//printf the menu to the serial console
	EnableInterrupts;							// Start catching heartbeat input now
	

	
	int VERBOSE_MODE = 1;
	uint8_t negative = 0;
	uint16_t speed = 0;			// Max value TOP_SPEED
	uint16_t power10[4] = {1000, 100, 10, 1};
	
	//main loop
	while(1)
	{
		EnableInterrupts;
		uint32_t stepGoToPos_deg = 90;//Each loop reset to zero
		
		if(COMMAND_ENTERED)
		{
		
			//Only execute after enter has been entered 
			//Think about implmenting a stop of the flow of data
			DisableInterrupts; //To avoid any interference when reading our serial data
			
			switch(SERIAL_OPTION)
			{
				
				case FLIP_CMD:
						clearInput();
						EnableInterrupts;					// Turn on after reading/writing
						ledFlag = 1;
				
						break;
					
				case PING_CMD:
						clearInput();
						EnableInterrupts;
						pingFlag = 1;
				
					break;

				case SERVO_CMD:
					
							if ((vportInput[4] == 'U') || (vportInput[4] == 'D')) {						// Guaranteed to be gt 0 since unsigned uints wrap around
								

							 if(vportInput[4] == 'U')
								{
								stepGoToPos_deg = getServoPos() + 9UL;
								}
								else
								{
								stepGoToPos_deg = getServoPos() - 9UL;
								}
							} // End if
						
							vportPrintf("SERVO: %d\n", stepGoToPos_deg);
						
							if ((stepGoToPos_deg > FULL_UP_DEG)) {
							vportPrintf("\n\rInput out of range! Servo isn't moving.\n\r");
							} // End if
						
							else
							{
							EnableInterrupts;
							servoSet(stepGoToPos_deg);
							}
					
					clearInput();
					EnableInterrupts;
				break;
				
				case STEPPER_CMD:
						clearInput();
						
						//Check for a vaild input
						if ((vportInput[4] == 'L') || (vportInput[4] == 'R')) 
							{						
								
									//Move stepper either left or right depending on the value
									if(vportInput[4] == 'L')
									{
										DisableInterrupts;
									stepGoToPos_deg = getStep() + 9UL; //Move by a set position
									}
									else
									{
										DisableInterrupts;
									stepGoToPos_deg = getStep() - 9UL; //9UL; //Move by a set position
									}
							} // End if
							else 
							{
								if(VERBOSE_MODE)
								{
								vportPrintf("\rInput isn't a number.\n");
								}
							} // End else
						
							
					
							
						//Check stepper position to determin if it can be moved
						if ((stepGoToPos_deg < 0) || (stepGoToPos_deg > FULL_RIGHT_DEG)) 
						{
							vportPrintf("\rInput too big!\n");
							
						} // End if
						
						else 
						{
							vportPrintf("Stepper %d\n", stepGoToPos_deg);
							EnableInterrupts; //Interrupts must be enabled to allow stepper to be moved
							stepGoToPos_step = (stepCount_step * stepGoToPos_deg)/FULL_RIGHT_DEG;
						}
				break;
						
				case DC_CMD:
					clearInput();
					
					if (vportInput[4] == '-')
						negative = 1;
					
					for(int i=4; i<=7; i++) {
						
						if ((vportInput[i + negative] - ASCII_0) < 10)
							speed += ((uint16_t)(vportInput[i + negative] - ASCII_0)) * power10[i-4];
						
						else {
							if (VERBOSE_MODE)
								vportPrintf("\rNot valid!\n");
							speed = SAME_SPEED;
						} // End else
					} // End for
						
					if(speed > TOP_SPEED) {
						if (VERBOSE_MODE)
							vportPrintf("\rInvalid speed\n");
					} // End if
					else {
				
						if ((vportInput[2] == 'L') || (vportInput[2] == 'R') || (vportInput[2] == 'B')) {
							
							if(vportInput[2] == 'L') {
								DisableInterrupts;
								
								if (negative)
									dirL = BWD;
								else
									dirL = FWD;
								
								if (0 == speed)
									dirL = LOCK;
								
								dcMotorSet(speed, SAME_SPEED);
							} // End if
							
							else if(vportInput[2] == 'R') {
								DisableInterrupts;
								
								if (negative)
									dirR = BWD;
								else
									dirR = FWD;
								
								if (0 == speed)
									dirR = LOCK;
								
								dcMotorSet(SAME_SPEED, speed);
							} // End if
							
							else {								// If both motors
								DisableInterrupts;
								
								if (negative) {
									dirL = BWD;
									dirR = BWD;
								} // End if
								else {
									dirL = FWD;
									dirR = FWD;
								} // End else
								
								if (0 == speed) {
									dirL = LOCK;
									dirR = LOCK;
								} // End if
								
								dcMotorSet(speed, speed);
							} // End if
						} // End if L or R or B
							
						else {
							if(VERBOSE_MODE)
								vportPrintf("\rInput isn't a number.\n");
						} // End else
					
					} // End else (the one for valid speed check)
					negative = 0;
					speed = 0;
					
				break;
						
				default:
			
						if (rxFlag)
							{
							vportPrintf("\n\rInput: Invalid\n\r");
							Delay_ms(10);						// Delay for writing
							clearInput();
							EnableInterrupts;
						}	// End if
						else {
							clearInput();
						} // End else
						DisableInterrupts;				// Turn on to check for new, valid input	
				}//end of switch
			
		}//end of if
	}//End of main loop
}//end of main




