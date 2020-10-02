/***************************************************************************
 * USART.h
 *
 * File containing the functions for running a serial menu for use with the
 * 		MCB project (S4W 2020)
 *
 * Caleb Hoeksema
 * May 2020
 **************************************************************************/


#ifndef		__USART_H
#define		__USART_H

// Header files
#include	"stm32f303xe.h"


// Constants
#define 	MAX_SER_BUF_SIZE		40

// Macros
#define		EnableInterrupts 		asm("ISB ; CPSIE I")
#define		DisableInterrupts		asm("CPSID I")

// Function prototypes
void usartInit(USART_TypeDef * USARTx);
void USART2_IRQHandler(void);


#endif
