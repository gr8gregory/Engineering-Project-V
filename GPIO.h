/*****************************************************************************
 * GPIO.h
 * Contains function prototypes to initialize a GPIO port
 * Caleb Hoeksema, Gregory Huras
 * April 2020
 ****************************************************************************/
 

#ifndef _GPIO_H
#define _GPIO_H

#include "SysClock.h"
#include "stm32f303xe.h"
#include "utils.h"


// Pin modes
#define		MODER_IN		0x0UL
#define		MODER_OUT		0x1UL
#define		MODER_AF		0x2UL
#define		MODER_AL		0x3UL

// Drain mode
#define 	PUSH_PULL		0x0UL
#define 	OPEN_DR			0x1UL

// Pin speed
#define		LOW_SPEED		0x0UL
#define		MED_SPEED		0x1UL
#define		FAST_SPEED	0x2UL
#define		HI_SPEED		0x3UL

// Pin pull up/down
#define		PULL_NONE		0x0UL
#define		PULL_UP			0x1UL
#define		PULL_DOWN		0x2UL
#define		PULL_RES		0x3UL

// Ports
#define		PORT_A			RCC_AHBENR_GPIOAEN

// GPIOA macros
#define		GPIOA_PIN_MODE(pin, mode)				FORCE_BITS(GPIOA->MODER, 3UL << ((pin)*2UL), (mode) << ((pin)*2UL))
#define 	GPIOA_PIN_DRV_TYPE(pin, type)		FORCE_BITS(GPIOA->ODR, 1 << (pin), (type) << (pin))
#define		GPIOA_OP_TYPE(pin, mode)				FORCE_BITS(GPIOA->OTYPER, 1UL << (pin), (mode) << (pin))
#define		GPIOA_PIN_SPEED(pin, speed)			FORCE_BITS(GPIOA->OSPEEDR, 3UL << (2*(pin)), (speed) << (2*(pin)))
#define		GPIOA_PIN_PULL(pin, pull)				FORCE_BITS(GPIOA->PUPDR, 3UL << (2*(pin)), (pull) << (2*(pin)))


#endif
