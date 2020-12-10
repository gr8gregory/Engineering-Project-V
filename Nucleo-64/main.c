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


#define SERIAL_OPTION (vportInput[1])	
#define COMMAND_ENTERED vportInput[vportOffset] == '\r'
#define SERVO_CMD ('T')
#define STEPPER_CMD ('S')
#define PING_CMD ('P')
#define FLIP_CMD ('F')

// Global variables
extern volatile uint8_t rxFlag;													// Indicates serial input (command input)
extern volatile uint8_t vportInput[MAX_SER_BUF_SIZE];		// Buffer containing command
extern volatile uint32_t vportOffset;										// Offset of current command
extern volatile uint8_t pingFlag;												// Indicates ping request
extern volatile uint8_t ledFlag;												// Indicates change LED state
extern volatile uint32_t stepGoToPos_step;							// Inidicates the position to send the stepper to
extern uint32_t stepCount_step;


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
	Heartbeat_Init();					// Set up the heartbeat timer (Currently reads any input-related ISRs)
	
	DisableInterrupts; 				// Don't want to catch anything from the heartbeat yet
	LED_Init();									// Set up LED flashing (if we need it)
	
	vportMenuPrint();							//printf the menu to the serial console
	EnableInterrupts;							// Start catching heartbeat input now
	

	
	int VERBOSE_MODE = 1;
	
	//main loop
	while(1)
	{
		EnableInterrupts;
		uint32_t stepGoToPos_deg = 0;//Each loop reset to zero
		
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
									stepGoToPos_deg = getStep() + 9; //Move by a set position
									}
									else
									{
										DisableInterrupts;
									stepGoToPos_deg = getStep() - 9; //9UL; //Move by a set position
									}
							} // End if
							else 
							{
								if(VERBOSE_MODE)
								{
								vportPrintf("\rInput isn't a number.\n");
								}
							} // End else
						
							
						vportPrintf("Stepper %u\n", stepGoToPos_deg);
							
						//Check stepper position to determin if it can be moved
						if ((stepGoToPos_deg < 0) || (stepGoToPos_deg > FULL_RIGHT_DEG)) 
						{
							vportPrintf("\rInput too big! Stepper isn't moving\n");
							
						} // End if
						
						else 
						{
							EnableInterrupts; //Interrupts must be enabled to allow stepper to be moved
							stepGoToPos_step = (stepCount_step * stepGoToPos_deg)/FULL_RIGHT_DEG;
						}
				break;
						
				default:
			
						if (rxFlag)
							{
							//vportPrintf("\n\rInput: Invalid\n\r");
							//Delay_ms(10);						// Delay for writing
							clearInput();
							//EnableInterrupts;
						}	// End if
						else {
							clearInput();
						} // End else
						DisableInterrupts;				// Turn on to check for new, valid input	
				}//end of switch
			
		}//end of if
	}//End of main loop
}//end of main




