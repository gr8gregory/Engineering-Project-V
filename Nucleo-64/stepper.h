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

#include "heartbeat.h"
#include "limit.h"


// Stepper Pins
#define		STEP_PIN_A			0UL
#define		STEP_PIN_B			1UL
#define		STEP_PIN_C			2UL
#define		STEP_PIN_D			3UL

// Stepper Port
#define		STEP_PORT				GPIOC
#define		STEP_CLK				RCC_AHBENR_GPIOCEN

// Step Type
#define 	FULL_STEP				2
#define		HALF_STEP				1

// Timer function prototypes
static void STEP_CLOCK_Init(void);
void stepper_Init(void);
void TIM3_IRQHandler(void);
void stepHome(void);

#endif
