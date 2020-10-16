<<<<<<< Updated upstream
=======
/*****************************************************************************
 * LED.h
 * 
 * Contains function prototypes to initialize an LED
 * 
 * Caleb Hoeksema, Gregory Huras, Andrew Sammut, Bill Stefanuk
 * October 2020
 ****************************************************************************/

>>>>>>> Stashed changes
#ifndef __STM32F303RET6_NUCLEO64_LED_H
#define __STM32F303RET6_NUCLEO64_LED_H

#include "stm32f303xe.h"

#define 	LED_PIN		5UL

void LED_Init(void);

void Red_LED_Off(void);
void Red_LED_On(void);
void Red_LED_Toggle(void);

void Green_LED_On(void);
void Green_LED_Off(void);
void Green_LED_Toggle(void);
#endif /* __STM32L476G_DISCOVERY_LED_H */
