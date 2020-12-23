/***************************************************************************
 * USART.h
 *
 * File containing the function prototypes for initializing serial (USART) ports
 *
 * Caleb Hoeksema, Gregory Huras, Andrew Sammut
 * October 2020
 **************************************************************************/


#ifndef		__USART_H
#define		__USART_H

// Header files
#include "GPIO.h"


// Constants
#define 	MAX_SER_BUF_SIZE		40
#define 	BAUD_RATE 				9600


// Function prototypes
void usartInit(USART_TypeDef * USARTx);


#endif
