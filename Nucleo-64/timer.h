/*****************************************************************************
 * timer.h
 * 
 * Contains function prototypes to initialize a timer
 * 
 * Caleb Hoeksema, Gregory Huras, Andrew Sammut
 * November 2020
 ****************************************************************************/
 

#ifndef _TIMER_H
#define _TIMER_H

#include "SysClock.h"
#include "stm32f303xe.h"
#include "utils.h"


// Counting Direction
#define 	TIM_CNT_UP		0x00UL
#define		TIM_CNT_DN		TIM_CR1_DIR_Pos

// Timer enable
#define 	TIMER_OFF			0x0UL
#define		TIMER_ON			TIM_CR1_CEN

// Main O/P Mode
#define		TIM_OP_OFF		0x0UL
#define		TIM_OP_ON			TIM_BDTR_MOE

// Pin speed

// Pin pull up/down

// Ports

// Timer macros
#define		TIMx_COUNT_DIR(timer, dir)		FORCE_BITS((timer->CR1), TIM_CR1_DIR, (dir))
// Nucleo: Count direction available on timers 1,2,3,4,8,200
#define		TIMx_ENABLE(timer, state)			FORCE_BITS((timer->CR1), TIM_CR1_CEN, (state))
// Nucleo: Enable available on timers 1,2,3,4,6,7,8,15,16,17,20
#define 	TIMx_OP_ENABLE(timer, mode)		FORCE_BITS((timer->BDTR), TIM_BDTR_MOE, (mode))
// Nucleo: MOE available on timers 1,8,15,16,17,20


/*
#define		TIM1_MODE(mode)					FORCE_BITS(TIM1->CCMR1, TIM_CCMR1_OC1M, (mode))
#define		TIM1_PRELOAD(mode)			FORCE_BITS(TIM1->CCMR1, TIM_CCMR1_OC1PE, (mode))
#define		TIM1_POLAR(mode)				FORCE_BITS(TIM1->CCER, TIM_CCER_CC1NP, (mode))

#define		COUNT_DIR(timer, dir)		FORCE_BITS((timer), TIM_CR1_DIR_Pos, (dir))
#define		OP_ENABLE(mode)					FORCE_BITS(TIM1->BDTR, TIM_BDTR_MOE, (mode))

#define 	GPIOx_PIN_MODE(port, pin, mode)				FORCE_BITS((port->MODER), 3UL << ((pin)*2UL), (mode) << ((pin)*2UL))
#define 	GPIOx_PIN_DRV_TYPE(port, pin, type)		FORCE_BITS((port->ODR), 1 << (pin), (type) << (pin))
#define		GPIOx_OP_TYPE(port, pin, mode)				FORCE_BITS((port->OTYPER), 1UL << (pin), (mode) << (pin))
#define		GPIOx_PIN_SPEED(port, pin, speed)			FORCE_BITS((port->OSPEEDR), 3UL << (2*(pin)), (speed) << (2*(pin)))
#define		GPIOx_PIN_PULL(port, pin, pull)				FORCE_BITS((port->PUPDR), 3UL << (2*(pin)), (pull) << (2*(pin)))
*/

// Function Prototypes

#endif
