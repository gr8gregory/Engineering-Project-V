/***************************************************************************
 * virtualPort.c
 *
 * File containing functions for running a virtual serial port for use with a
 * 		terminal emulator
 *
 * Caleb Hoeksema
 * October 2020
 **************************************************************************/

// Header file
#include "virtualPort.h"


// Global Variables
volatile uint8_t vportInput[MAX_SER_BUF_SIZE];	// Stores virtual port input
volatile uint32_t vportOffset = 0;							// Stores virtual port input offset
volatile uint8_t rxFlag = 0;										// Indicates if there is input

//A simple function that prints the menu to serial
void vportMenuPrint(void)
{
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
		vportPrintf("\n");

	
	// Add other commands as needed
	Delay_ms(20);									// Delay to write
}

// Function to initialize a virtual port for control and debugging
	// For STM32F303RET6 (Nucleo64), PA2 is Tx and PA3 is Rx (USART2)
void virtualPortInit(void) {
	
	// ---------------------------------------------------------------
	// Start the USART2 clock
	SET_BITS(RCC->APB1ENR, RCC_APB1ENR_USART2EN);
	
	// Select the USART2 clock source
	// 00: PCLK selected as USART2 clock
	// 01: System clock (SYSCLK) selected as USART2 clock
	// 10: HSI16 clock selected as USART2 clock
	// 11: LSE clock selected as USART2 clock
	FORCE_BITS( RCC->CFGR3, RCC_CFGR3_USART2SW, RCC_CFGR3_USART2SW_SYSCLK );
	// ------------------------------------------------------------------
	
	// Configure the required pins	
	GPIO_clock_enable(VPORT_CLK);
	
	// Set pin mode (AF7 - USART2)
	GPIOx_PIN_MODE(VIRT_PORT, VPORT_TX_PIN, MODER_AF);
	GPIOx_PIN_MODE(VIRT_PORT, VPORT_RX_PIN, MODER_AF);
	GPIOx_AF_MODE(VIRT_PORT, VPORT_TX_PIN, VPORT_AF7);
	GPIOx_AF_MODE(VIRT_PORT, VPORT_RX_PIN, VPORT_AF7);
	
	// Set pin speed
	GPIOx_PIN_SPEED(VIRT_PORT, VPORT_TX_PIN, HI_SPEED);
	GPIOx_PIN_SPEED(VIRT_PORT, VPORT_RX_PIN, HI_SPEED);
	
	// Set push pull
	GPIOx_PIN_PULL(VIRT_PORT, VPORT_TX_PIN, PULL_UP);
	GPIOx_PIN_PULL(VIRT_PORT, VPORT_RX_PIN, PULL_UP);
	
	// Set output type
	GPIOx_PIN_DRV_TYPE(VIRT_PORT, VPORT_TX_PIN, PUSH_PULL);
	GPIOx_PIN_DRV_TYPE(VIRT_PORT, VPORT_RX_PIN, PUSH_PULL);
	
	// Initialize USART2
	usartInit(USART2);
	
}	// End virtualPortInit();


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
		vportPutc(*str++);											// Pass the string char by char
	
}	// End vportPuts()


// Private character output to serial port
static void vportPutc(char c) {
	
	while (!(USART2->ISR & USART_ISR_TXE));		// Wait for H/W
	USART2->TDR = (uint8_t)c;									// Data output
	
}	// End vportPutc()


// Interrupt handler for virtual port recieve
void USART2_IRQHandler(void) {
	
	uint8_t junk = 0;													// For catching invalid requests (may cause a warning)
	if (USART2->ISR & USART_ISR_RXNE) {				// Check if there is actually something there
		
		vportOffset++;													// Move to next byte in buffer
		if (vportOffset >= MAX_SER_BUF_SIZE)		// Overwrite the start of the buffer
			vportOffset = 0;
		
		rxFlag++;																// Set flag
		vportInput[vportOffset] = USART2->RDR;	// Read, acknowledge ISR
		
	}	// End if
	
	junk = USART2->RDR;												// Acknowledge the interrupt request if called invalidly
	
}	// End USART2_IRQHandler()

void clearInput(void) 
{
	
	rxFlag = 0;										// Reset after reading
	vportInput[vportOffset] = 0;	// Delete return char
	vportOffset = 0;							// Reset the command input	
	vportInput[1] = 0;						// Wipe scanned char to avoid infinite loop
	
} // End clearInput

