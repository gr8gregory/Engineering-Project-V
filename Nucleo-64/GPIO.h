/*****************************************************************************
 * GPIO.h
 * 
 * Contains function prototypes to initialize a GPIO port
 * 
 * Caleb Hoeksema, Gregory Huras, Andrew Sammut
 * October 2020
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

// GPIOx macros
#define 	GPIOx_PIN_MODE(port, pin, mode)				FORCE_BITS((port->MODER), 3UL << ((pin)*2UL), (mode) << ((pin)*2UL))
#define 	GPIOx_PIN_DRV_TYPE(port, pin, type)		FORCE_BITS((port->ODR), 1UL << (pin), (type) << (pin))
#define		GPIOx_OP_TYPE(port, pin, mode)				FORCE_BITS((port->OTYPER), 1UL << (pin), (mode) << (pin))
#define		GPIOx_PIN_SPEED(port, pin, speed)			FORCE_BITS((port->OSPEEDR), 3UL << (2UL*(pin)), (speed) << (2UL*(pin)))
#define		GPIOx_PIN_PULL(port, pin, pull)				FORCE_BITS((port->PUPDR), 3UL << (2UL*(pin)), (pull) << (2UL*(pin)))
#define 	GPIOx_AF_MODE(port, pin, mode)				FORCE_BITS((port->AFR[0]), 0xFUL << ((pin)*4UL), (mode) << ((pin)*4UL))

// Interrupt Macros
#define		EnableInterrupts 		__asm("ISB ; CPSIE I")
#define		DisableInterrupts		__asm("CPSID I")


// Function Prototypes
void GPIO_clock_enable(volatile uint32_t);
void Delay_ms(uint32_t);

#endif
