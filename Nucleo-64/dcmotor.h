/*****************************************************************************
 * dcmotor.h
 * 
 * Contains function prototypes to initialize a pair of DC motors.
 * 
 * Caleb Hoeksema, Gregory Huras, Andrew Sammut
 * December 2020
 ****************************************************************************/
 

#ifndef _DCMOTOR_H
#define _DCMOTOR_H

#include "timer.h"
#include "GPIO.h"


// Inividual Motor settings
#define 	OFF							0x0UL
#define 	FWD							0x1UL
#define		BWD							0x2UL
#define		LOCK						0x3UL

#define		TOP_SPEED				3000UL
#define 	SAME_SPEED			TOP_SPEED + 1


// DC Pins
#define		DC_DRV_L_PIN		10UL
#define		DC_DRV_R_PIN		11UL

#define		DC_DIR_FL_PIN		8UL
#define		DC_DIR_BL_PIN		9UL

#define		DC_DIR_FR_PIN		8UL
#define		DC_DIR_BR_PIN		9UL

// DC Port Info
#define		DC_DRV_PORT			GPIOC
#define		DC_DRV_CLK			RCC_AHBENR_GPIOCEN
#define 	DC_DRV_TIM			TIM8									// Channel 1 for L, 2 for R
#define		DC_DRV_AF4			0x4UL

#define		DC_DIR_L_PORT		GPIOB
#define		DC_DIR_L_CLK		RCC_AHBENR_GPIOBEN

#define		DC_DIR_R_PORT		GPIOC
#define		DC_DIR_R_CLK		RCC_AHBENR_GPIOCEN


// DC motor function prototypes
void DC_DRV_Init(void);
void DC_DRV_clock_Init(void);
void dcMotorSet(uint16_t left, uint16_t right);
void DC_DIR_Init(void);

#endif
