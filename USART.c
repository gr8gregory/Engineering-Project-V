/***************************************************************************
 * USART.c
 *
 * File containing the functions for running a serial menu for use with the
 * 		MCB project (S4W 2020)
 *
 * Caleb Hoeksema
 * May 2020
 **************************************************************************/


// Header file
#include "USART.h"

// Global variables
volatile uint8_t vportInput[MAX_SER_BUF_SIZE];	// Stores virtual port input
volatile uint32_t vportOffset = 0;							// Stores virtual port input offset
uint8_t rxFlag = 0;															// Indicates if there is input


// Function to initialize a generic USART port
void usartInit(USART_TypeDef * USARTx) {
	
	USARTx->CR1 &= ~USART_CR1_UE;									// Disables the USART port
	USARTx->CR1 &= ~USART_CR1_M;
	
	USARTx->CR2 &= ~USART_CR2_STOP;
	USARTx->CR1 &= ~USART_CR1_PCE;
	USARTx->CR1 &= ~USART_CR1_OVER8;

	USARTx->BRR = 0x208D;													// No idea where this is from
	
	USARTx->CR1 |= (USART_CR1_TE | USART_CR1_RE);
	
	// Enable interrupt for virtual port
	if (USARTx == USART2) {
		
		USART2->CR1 |= USART_CR1_RXNEIE;
		NVIC_SetPriority(USART2_IRQn, 0);
		NVIC_EnableIRQ(USART2_IRQn);
		
	}
		
	USARTx->CR1 |= USART_CR1_UE;
	
	// Wait for hardware
	while((USARTx->ISR & USART_ISR_TEACK)==0);
	while((USARTx->ISR & USART_ISR_REACK) == 0);
	
}	// End usartInit()


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
