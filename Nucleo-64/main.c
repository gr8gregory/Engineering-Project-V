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


// Mainline function
int main(void) {
	
	// Initialization
	System_Clock_Init();			// Set up clock
	SystemCoreClockUpdate();	// Set up clock
	
	virtualPortInit();				// Set up the terminal (Currently on an ISR)
	
	limit_Init();							// Set up limit switches
	stepper_Init();						// Set up stepper motor
	
	servo_Init();
	
	Heartbeat_Init();					// Set up the heartbeat timer (Currently reads any input-related ISRs)
	DisableInterrupts; 				// Don't want to catch anything from the heartbeat yet
	
	// Todo
		// Need to initialize and home the servo motor
	
	// Todo Later:
		// Initialize the encoders
		// Initialize the DC motors

	LED_Init();									// Set up LED flashing (if we need it)
	
	// Start feeding terminal	
	VP_RESET;
	VP_CLEAR;
	vportPrintf("MCB Controller Menu\n\r");
	vportPrintf("\tSeptember 2020, Ver. 3.0\n\n\r");
	vportPrintf("Command instructions\n\r");
	vportPrintf("Flip LED: f,F\n\r");
	vportPrintf("Ping Board: P (should return \"p\")\n\r");
	vportPrintf("Move stepper motor: S #### (leading 0s)\n\r");
	vportPrintf("Move stepper motor: T #### (leading 0s)\n\r");
	vportPrintf("Press return to terminate ALL commands\r\n\n");
	// Add other commands as needed
	Delay_ms(20);									// Delay to write
	
	EnableInterrupts;							// Start catching heartbeat input now
	
	// Process loop
	while(1) 
		prompt();										// Read any command input
	
}


// Function to parse input
uint8_t prompt(void) {
	
	// Variable to catch bad input
	uint8_t error = 1;
	uint32_t stepGoToPos_deg = 0;
	uint16_t pow_ten[4] = {1000, 100, 10, 1};
	
	// Catch commands once they are complete (once return is typed)
	if (vportInput[vportOffset] == '\r') {	
		
		while (error) {
			
			DisableInterrupts;							// Turn off so nothing is added while reading input
				
				// Check command type (first char of input)
				switch (vportInput[1]) { 			// The ISR steps vportOffset before writing so the 
																				// first valid input is the second char in vportInput
					
					// Check for LED command (Flip LD2)
					case ('f'):
					case ('F'):
						clearInput();
						EnableInterrupts;					// Turn on after reading/writing
						ledFlag = 1;
						error = 0;
						break;
					
					// To ping the Nucleo (capital P, should return lowercase)
					case ('P'):
						clearInput();
						EnableInterrupts;
						pingFlag = 1;
						error = 0;
						break;
					
					// Expects and ASSUMES input in the form " S ####"
					case('T'):
						clearInput();
						EnableInterrupts;
					
						for(int i = 3; i < 7; i++) { 
							
							if ((vportInput[i] - ASCII_0) < 10) {						// Guaranteed to be gt 0 since unsigned uints wrap around
								stepGoToPos_deg += (vportInput[i] - ASCII_0) * pow_ten[i-3];
								error = 0;
							} // End if
							
							else {
								vportPrintf("\n\rInput isn't a number.\n\r");
								error = 1;
							} // End else
						} // End for
						
						vportPrintf("\n\rt %d\n\r", stepGoToPos_deg);
						
						if ((stepGoToPos_deg > FULL_UP_DEG)) {
							vportPrintf("\n\rInput out of range! Servo isn't moving.\n\r");
							error = 1;
						} // End if
						
						else {
							servoSet(stepGoToPos_deg);
							// stepGoToPos_step = (stepCount_step * stepGoToPos_deg)/FULL_RIGHT_DEG;
							error = 0;
						}
						break;
						
					// Expects and ASSUMES input in the form S ####
					case('S'):
						clearInput();
						EnableInterrupts;
						
						for(int i = 3; i < 7; i++) { 
							
							if ((vportInput[i] - ASCII_0) < 10) {						// Guaranteed to be gt 0 since unsigned uints wrap around
								stepGoToPos_deg += (vportInput[i] - ASCII_0) * pow_ten[i-3];
								error = 0;
							} // End if
							
							else {
								vportPrintf("\n\rInput isn't a number.\n\r");
								error = 1;
							} // End else
						} // End for
						
						vportPrintf("\n\rs %d\n\r", stepGoToPos_deg);
						
						if (stepGoToPos_deg > FULL_RIGHT_DEG) {
							vportPrintf("\n\rInput too big! Stepper isn't moving.\n\r");
							error = 1;
						} // End if
						
						else {
							stepGoToPos_step = (stepCount_step * stepGoToPos_deg)/FULL_RIGHT_DEG;
							error = 0;
						}
						break;
					
					default:	
						if (rxFlag) {
							vportPrintf("\n\rInput: Invalid\n\r");
							Delay_ms(20);						// Delay for writing
							clearInput();
							EnableInterrupts;
							error = 1;							// Report invalid input
						}	// End if
						else {
							error = 1;
							clearInput();
						} // End else
						DisableInterrupts;				// Turn on to check for new, valid input	
					
				} // End switch
				
			EnableInterrupts;
		} // End while
	}	// End if
	
	return(0);
} // End prompt


// Private function to clear anything related to terminal input
	// Simply to declutter prompt()
static void clearInput(void) {
	
	rxFlag = 0;										// Reset after reading
	vportInput[vportOffset] = 0;	// Delete return char
	vportOffset = 0;							// Reset the command input	
	vportInput[1] = 0;						// Wipe scanned char to avoid infinite loop
	
} // End clearInput

