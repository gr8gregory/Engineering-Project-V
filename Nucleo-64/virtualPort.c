/***************************************************************************
 * virtualPort.c
 *
 * File containing functions for running a virtual serial port for use with a
 * 		terminal emulator
 *
 * Caleb Hoeksema
 * October 2020
 **************************************************************************/

<<<<<<< Updated upstream

// Header file
#include "virtualPort.h"
=======
// Header file
#include "virtualPort.h"


// Global Variables
volatile uint8_t vportInput[MAX_SER_BUF_SIZE];	// Stores virtual port input
volatile uint32_t vportOffset = 0;							// Stores virtual port input offset
volatile uint8_t rxFlag = 0;										// Indicates if there is input
>>>>>>> Stashed changes


// Function to initialize a virtual port for control and debugging
	// For STM32F303RET6 (Nucleo46), PA2 is Tx and PA3 is Rx (USART2)
void virtualPortInit(void) {
	
	// Start GPIOA clock
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	
	// Configure the required pins
	gpioConfig();	
	
	// Start the USART2 clock
	SET_BITS(RCC->APB1ENR, RCC_APB1ENR_USART2EN);
	
	// Set the USART2 clock frequency
	CLR_BITS(RCC->CFGR3, RCC_CFGR3_USART2SW);
	SET_BITS(RCC->CFGR3, RCC_CFGR3_USART2SW_0);
	
	// Initialize USART2
	usartInit(USART2);
	
}	// End virtualPortInit();


<<<<<<< Updated upstream
// Private function to initialize GPIOD for a virtual port
static void gpioConfig(void) {
	
	// Set pin mode (AF7 - USART2)
	GPIOA_PIN_MODE(VPORT_TX_PIN, MODER_AF);
	GPIOA_PIN_MODE(VPORT_RX_PIN, MODER_AF);
	GPIOA->AFR[0] &= ~(0x3UL)<<(5 *4);
	GPIOA->AFR[0] &= ~(0x3UL)<<(6 *4);
	GPIOA->AFR[0] |= 7UL<<(5 *4);
	GPIOA->AFR[0] |= 7UL<<(6 *4);
	
	// Set pin speed
	GPIOA_PIN_SPEED(VPORT_TX_PIN, HI_SPEED);
	GPIOA_PIN_SPEED(VPORT_RX_PIN, HI_SPEED);
	
	// Set push pull
	GPIOA_PIN_PULL(VPORT_TX_PIN, PULL_UP);
	GPIOA_PIN_PULL(VPORT_RX_PIN, PULL_UP);
	
	// Set output type
	GPIOA_PIN_DRV_TYPE(VPORT_TX_PIN, PUSH_PULL);
	GPIOA_PIN_DRV_TYPE(VPORT_RX_PIN, PUSH_PULL);
	
}	// End gpioConfig()
=======
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
	
}	// End USART2_IRQHandler
>>>>>>> Stashed changes


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
