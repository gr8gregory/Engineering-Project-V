/***************************************************************************
 * USART.c
 *
 * File containing the functions for initializing a serial (USART) port
 *
 * Caleb Hoeksema, Gregory Huras, Andrew Sammut
 * October 2020
 **************************************************************************/

// Header file
#include "USART.h"
#include "GPIO.h"


// Function to initialize a generic USART port
void usartInit(USART_TypeDef * USARTx) {
	
	USARTx->CR1 &= ~USART_CR1_UE;									// Disables the USART port
	USARTx->CR1 &= ~USART_CR1_M;
	
	USARTx->CR2 &= ~USART_CR2_STOP;
	// USARTx->CR1 &= ~USART_CR1_PCE;
	USARTx->CR1 &= ~USART_CR1_OVER8;

	// CSet Baudrate to 9600 using APB frequency (72,000,000 Hz)
	// If oversampling by 16, Tx/Rx baud = f_CK / USARTDIV,  
	// If oversampling by 8,  Tx/Rx baud = 2*f_CK / USARTDIV
  // When OVER8 = 0, BRR = USARTDIV
	// USARTDIV = 72MHz/9600 = 7500
	// BRR limited to 16 bits
	USARTx->BRR = SystemCoreClock/BAUD_RATE;
	
	USARTx->CR1 |= (USART_CR1_TE | USART_CR1_RE);
	
	// Enable interrupt for virtual port
	if (USARTx == USART2) {
		
		USART2->CR1 |= USART_CR1_RXNEIE;
		NVIC_SetPriority(USART2_IRQn, 0);
		NVIC_EnableIRQ(USART2_IRQn);
		
	} // End if
		
	USARTx->CR1 |= USART_CR1_UE;
	
	// Wait for hardware
	while((USARTx->ISR & USART_ISR_TEACK)==0);
	while((USARTx->ISR & USART_ISR_REACK) == 0);
	
}	// End usartInit()


