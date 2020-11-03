/*****************************************************************************
 * LED.h
 * 
 * Contains function prototypes to initialize an LED
 * 
 * Caleb Hoeksema, Gregory Huras, Andrew Sammut, Bill Stefanuk
 * October 2020
 ****************************************************************************/

#ifndef __STM32F303RET6_NUCLEO64_LED_H
#define __STM32F303RET6_NUCLEO64_LED_H

#include "stm32f303xe.h"


// Pins
#define 	LED_PIN		5UL


// Function Prototypes
void LED_Init(void);

void Red_LED_Off(void);
void Red_LED_On(void);
void Red_LED_Toggle(void);

#endif /* __STM32L476G_DISCOVERY_LED_H */
