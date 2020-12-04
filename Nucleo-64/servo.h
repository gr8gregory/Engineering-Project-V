/*****************************************************************************
 * servo.h
 * 
 * Contains function prototypes to initialize a servomotor
 * 
 * Caleb Hoeksema, Gregory Huras, Andrew Sammut
 * November 2020
 ****************************************************************************/
 

#ifndef _SERVO_H
#define _SERVO_H

#include "timer.h"
#include "GPIO.h"


// Servo Pin
#define		SERVO_PIN				0UL

// Servo Port
#define		SERVO_PORT			GPIOB
#define		SERVO_CLK				RCC_AHBENR_GPIOBEN
#define 	SERVO_TIM				TIM1
#define		SERVO_AF6				0x6UL

// PWM Constants
#define		PULSE_MIN				600UL			// In useconds, corresponds to 0 deg
#define		PULSE_MID				1500UL		// In useconds, corresponds to 90 deg
#define		PULSE_MAX				2400UL		// In useconds, corresponds to 180 deg

// Servo Limits
#define 	FULL_DN_DEG			0UL
#define 	FULL_UP_DEG			180UL

// Timer function prototypes
void servo_Init(void);
void servoSet(uint16_t);
static void servo_clock_Init(void);

#endif
