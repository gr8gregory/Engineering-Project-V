/*****************************************************************************
 * Heartbeat.c
 * 
 * Contains functions initialize a heartbeat timer and deal with related IRQs
 * 
 * Caleb Hoeksema, Gregory Huras, Andrew Sammut
 * October 2020
 ****************************************************************************/

/* 
 * THE HEARTBEAT TIMER IS ON TIMER 2.
 */
 
 
// Header files
#include "heartbeat.h"

// Globals
volatile uint8_t pingFlag = 0;
volatile uint8_t ledFlag = 0;


// Initalize the Timer on PA0 (Timer 2) and set it to trigger an interrupt
	// For now, this is the way to act on interrupts indicated by global vars
void Heartbeat_Init(void) {
	
	// NON-GENERIC------------------------------------------------
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	
	// Disable Timer
	TIMx_ENABLE(HRT_TIM, TIMER_OFF);
	
	// Clock Prescale (16 bits - up to 65 535)
	HRT_TIM->PSC = 71;					// 72MHz clock --> clock/(PSC+1) = 1MHz, matches useconds
	
	// Auto-reload (also 16b)
	HRT_TIM->ARR = 2000-1;			// 1MHz clock (see above), period = 2ms --> ARR = clock*period - 1
	
	// Count direction
	TIMx_COUNT_DIR(HRT_TIM, TIM_CNT_UP);
	
	// NON-GENERIC------------------------------------------------
	// Enable interrupts
	SET_BITS(TIM2->DIER, TIM_DIER_UIE);
	
	// Enable TIM2 in NVIC
	NVIC_EnableIRQ(TIM2_IRQn);
	// -----------------------------------------------------------
	
	// Enable Timer
	TIMx_ENABLE(HRT_TIM, TIMER_ON);
	
} // End Heartbeat_Init()


// Timer 2 IRQ (Heartbeat timer)
// Function name assumes timer number
void TIM2_IRQHandler(void) {
	
	// Check if the timer has overflowed
	if((HRT_TIM->SR & TIM_SR_UIF) != 0) {
		
		// Check for ping request
		if (pingFlag) {
			vportPrintf("PING!\n");
			pingFlag = 0;				// Clear
		} // End if
		
		// Check for LED flip request
		if(ledFlag) {
			LED_Toggle();			// Not actually red but whatever
			ledFlag = 0;
		} // End if
		
	} // End if
	HRT_TIM->SR &= ~TIM_SR_UIF; 			// Clear interrupt request to clear
	
} // End TIM2_IRQHandler()
