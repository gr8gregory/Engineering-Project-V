/*****************************************************************************
 * Heartbeat.h
 * 
 * Contains function prototypes to initialize a heartbeat timer
 * 
 * Caleb Hoeksema, Gregory Huras, Andrew Sammut
 * October 2020
 ****************************************************************************/
 

#ifndef _HEARTBEAT_H
#define _HEARTBEAT_H

#include "SysClock.h"
#include "stm32f303xe.h"
#include "timer.h"

#include "virtualPort.h" 		// For vportPrintf()
#include "LED.h"						// For Red_LED_Toggle()


// Definitions
#define 	HRT_TIM				TIM2


// Timer function prototypes
void Heartbeat_Init(void);
void TIM2_IRQHandler(void);

#endif
