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

#include "stepper.h"


// Limit Pins
#define		LEFT_LIM_PIN		8UL
#define		RIGHT_LIM_PIN		9UL

// Limit Port
#define		LIMIT_PORT			GPIOA
#define		LIMIT_CLK				RCC_AHBENR_GPIOAEN

// Limits
#define 	FULL_LEFT				0UL
#define 	FULL_RIGHT			1800UL

// Timer function prototypes
void limit_Init(void);
void EXTI9_5_IRQHandler(void);
// static void STEP_CLOCK_Init(void);
// void stepper_Init(void);
// void TIM3_IRQHandler(void);
// void stepHome(void);

#endif
