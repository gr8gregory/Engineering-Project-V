/*****************************************************************************
 * stepper.h
 * 
 * Contains function prototypes to initialize a stepper motor
 * 
 * Caleb Hoeksema, Gregory Huras, Andrew Sammut
 * October 2020
 ****************************************************************************/
 

#ifndef _STEPPER_H
#define _STEPPER_H

#include "timer.h"
#include "GPIO.h"
#include "limit.h"

// Stepper Pins
#define		STEP_PIN_A			0UL
#define		STEP_PIN_B			1UL
#define		STEP_PIN_C			2UL
#define		STEP_PIN_D			3UL

// Stepper Port
#define		STEP_PORT				GPIOC
#define		STEP_CLK				RCC_AHBENR_GPIOCEN
#define 	STEP_TIM				TIM3

// Step Type
#define 	FULL_STEP				2
#define		HALF_STEP				1

// Timer function prototypes
void stepper_Init(void);
void stepHome(void);
static void step_clock_Init(void);
void TIM3_IRQHandler(void);
uint32_t getStep(void );
#endif
