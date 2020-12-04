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
#define 	LED_PORT	GPIOA

#define		LED_OFF		0x0UL
#define		LED_ON		0x1UL

// Function Prototypes
void LED_Init(void);

void LED_Off(void);
void LED_On(void);
void LED_Toggle(void);

#endif /* __STM32L476G_DISCOVERY_LED_H */
