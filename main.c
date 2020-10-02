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

// Global variable
extern uint8_t rxFlag;
extern volatile uint8_t vportInput[MAX_SER_BUF_SIZE];
extern volatile uint32_t vportOffset;

// Function Prototypes
uint8_t prompt(void);


// Mainline function
int main(void) {
	
	// Initialization
	System_Clock_Init();			// Womp womp...
	virtualPortInit();
	
	while(1) 
		prompt();
	
}

uint8_t prompt(void) {
	
	uint8_t error = 1;
	
	VP_RESET;
	VP_CLEAR;
	vportPrintf("Flip the LED: <F>\n");
	Delay_ms(20);
	
	while (error) {
		
		DisableInterrupts;
		
			switch (vportInput[vportOffset]) {
				
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
