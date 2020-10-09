/***************************************************************************
 * virtualPort.c
 *
 * File containing functions for running a virtual serial port for use with a
 * 		terminal emulator
 *
 * Caleb Hoeksema, Gregory Huras, Andrew Sammut
 * October 2020
 **************************************************************************/

// Header files
#include "virtualPort.h"


// Global Variables
volatile uint8_t vportInput[MAX_SER_BUF_SIZE];		// Stores virtual port input
volatile uint32_t vportOffset = 0;					// Stores virtual port input offset
uint8_t rxFlag = 0;									// Indicates if there is input


// Function to initialize a virtual port for control and debugging
	// For STM32F303RET6 (Nucleo46), PA2 is Tx and PA3 is Rx (USART2)
void virtualPortInit(void) {
	
	// Start the USART2 clock
	SET_BITS(RCC->APB1ENR, RCC_APB1ENR_USART2EN);
	
	// Select the USART2 clock source
	// 00: PCLK selected as USART2 clock
	// 01: System clock (SYSCLK) selected as USART2 clock
	// 10: HSI16 clock selected as USART2 clock
	// 11: LSE clock selected as USART2 clock
	FORCE_BITS( RCC->CFGR3, RCC_CFGR3_USART2SW, RCC_CFGR3_USART2SW_SYSCLK );
	
	// Configure the required pins
	GPIOA_Init();
	
	// Initialize USART2
	usartInit(USART2);
	
} // End virtualPortInit();


// Interrupt handler for virtual port recieve
void USART2_IRQHandler(void) {
	
	uint8_t junk = 0;							// For catching invalid requests (may cause a warning)
	if (USART2->ISR & USART_ISR_RXNE) {			// Check if there is actually something there
		
		vportOffset++;							// Move to next byte in buffer
		if (vportOffset >= MAX_SER_BUF_SIZE)	// Overwrite the start of the buffer
			vportOffset = 0;
		
		rxFlag++;								// Set flag
		vportInput[vportOffset] = USART2->RDR;	// Read, acknowledge ISR
		
	}	// End if
	
	junk = USART2->RDR;							// Acknowledge the interrupt request if called invalidly
	
}	// End USART2_IRQHandler


/* Functions for sending data from the processor to the terminal */

// Function to send data through the serial port
void vportPrintf(char *fmt, ...) {
	
	va_list args;
	char buffer[MAX_SER_BUF_SIZE];
	
	va_start(args, fmt);
	vsnprintf(buffer, MAX_SER_BUF_SIZE, fmt, args);
	va_end(args);
	
	vportPuts(buffer);
	
}	// End vportPrintf()


// Private string output for serial port
static void vportPuts(char *str) {
	
	while(*str)
		vportPutc(*str++);						// Pass the string char by char
	
}	// End vportPuts()


// Private character output to serial port
static void vportPutc(char c) {
	
	while (!(USART2->ISR & USART_ISR_TXE));		// Wait for H/W
	USART2->TDR = (uint8_t)c;					// Data output
	
}	// End vportPutc()
