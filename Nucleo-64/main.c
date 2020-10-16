/***************************************************************************
 * main.c
 *
 * Filp an LED on the Nucleo64 over the virtual port
 *
 * Caleb Hoeksema
 * October 2020
 **************************************************************************/


// Header files
#include "virtualPort.h"
#include "USART.h"
#include "LED.h"
<<<<<<< Updated upstream

// Global variable
extern uint8_t rxFlag;
extern volatile uint8_t vportInput[MAX_SER_BUF_SIZE];
extern volatile uint32_t vportOffset;
=======
#include "Heartbeat.h"
#include "stepper.h"
#include "limit.h"

// Global variables
extern volatile uint8_t rxFlag;													// Indicates serial input (command input)
extern volatile uint8_t vportInput[MAX_SER_BUF_SIZE];		// Buffer containing command
extern volatile uint32_t vportOffset;										// Offset of current command
extern volatile uint8_t pingFlag;												// Indicates ping request
extern volatile uint8_t ledFlag;												// Indicates change LED state
>>>>>>> Stashed changes

// Function Prototypes
uint8_t prompt(void);


// Mainline function
int main(void) {
	
	// Initialization
<<<<<<< Updated upstream
	System_Clock_Init();			// Womp womp...
	virtualPortInit();
	
	while(1) 
		prompt();
=======
	System_Clock_Init();			// Set up clock
	SystemCoreClockUpdate();	// Set up clock
	
	virtualPortInit();				// Set up the terminal (Currently on an ISR)
	
	limit_Init();
	stepper_Init();	
	
	Heartbeat_Init();					// Set up the heartbeat timer (Currently reads any input-related ISRs)
	DisableInterrupts; 				// Don't want to catch anything from the heartbeat yet
	
	// Todo
		// Need to initialize the limit switches (ISRs)
		// Need to initialize and home the stepper motor
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
	vportPrintf("Press return to terminate ALL commands\n\n\r");
	// Add other commands as needed
	
	vportPrintf("Your input: ");	// Assuming you are echoing back (but still can look awful)
	Delay_ms(20);									// Delay to write
	
	EnableInterrupts;							// Start catching heartbeat input now
	
	// Process loop
	while(1) 
		prompt();										// Read any command input
>>>>>>> Stashed changes
	
}

uint8_t prompt(void) {
	
	uint8_t error = 1;
	
	VP_RESET;
	VP_CLEAR;
	vportPrintf("Flip the LED: <F>\n");
	Delay_ms(20);
	
	while (error) {
		
<<<<<<< Updated upstream
		DisableInterrupts;
		
			switch (vportInput[vportOffset]) {
=======
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
					
					default:	
						if (rxFlag) {
							vportPrintf("Input: Invalid");
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
>>>>>>> Stashed changes
				
				case ('f'|'F'):
					rxFlag = 0;										// Reset after reading
					vportInput[vportOffset] = 0;	//Clear to avoid infinite loop
					EnableInterrupts;							// Turn on after reading/writing
					Red_LED_Toggle();
					break;
				default:	
					if (rxFlag) {
						vportPrintf("\rInput: Invalid");
						Delay_ms(20);								// Delay for writing
						rxFlag = 0;
						vportInput[vportOffset] = 0;
						error = 1;									// Report invalid input
					}	// End if
					else
						error = 1;
			} // End switch
			
		EnableInterrupts;
	} // End while
	
	return(0);
} // End prompt
<<<<<<< Updated upstream
=======


// Private function to clear anything related to terminal input
	// Simply to declutter prompt()
static void clearInput(void) {
	
	rxFlag = 0;										// Reset after reading
	vportInput[vportOffset] = 0;	// Delete return char
	vportOffset = 0;							// Reset the command input	
	vportInput[1] = 0;						// Wipe scanned char to avoid infinite loop
	
} // End clearInput

>>>>>>> Stashed changes
