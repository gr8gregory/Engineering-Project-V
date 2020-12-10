/*****************************************************************************
 * limit.h
 * 
 * Contains function prototypes to initialize limit switches
 * 
 * Caleb Hoeksema, Gregory Huras, Andrew Sammut
 * October 2020
 ****************************************************************************/
 

#ifndef _LIMIT_H
#define _LIMIT_H

#include "timer.h"
#include "GPIO.h"
#include "stepper.h"


// Limit Pins
#define		LEFT_LIM_PIN		5UL
#define		RIGHT_LIM_PIN		6UL

// Limit Port
#define		LIMIT_PORT			GPIOC
#define		LIMIT_CLK				RCC_AHBENR_GPIOCEN

// Limits
#define 	FULL_LEFT_DEG		0UL
#define 	FULL_RIGHT_DEG	1800UL

// Timer function prototypes
void limit_Init(void);
void EXTI9_5_IRQHandler(void);

#endif
