/*****************************************************************************
 * Heartbeat.c
 * 
 * Contains functions initialize a heartbeat timer and deal with related IRQs
 * 
 * Caleb Hoeksema, Gregory Huras, Andrew Sammut
 * October 2020
 ****************************************************************************/

// Header files
#include "heartbeat.h"

// Globals
volatile uint8_t pingFlag = 0;
volatile uint8_t ledFlag = 0;


// Function to enable timer clock
	// Update to be more generic? Macro?
void TIMER_CLOCK_ENABLE(void) {
	
	// Set timer 1 clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	
} // End TIMER_CLOCK_ENABLE


// Initalize the Timer on PA0 (Timer 2) and set it to trigger an interrupt
	// For now, this is the way to act on interrupts indicated by global vars
void Heartbeat_Init(void) {
	
	TIMER_CLOCK_ENABLE();
	
	// Disable Timer
	CLR_BITS(TIM2->CR1, TIM_CR1_CEN);
	
	// Clock Prescale (16 bits - up to 65 535)
	TIM2->PSC = 71;					// 72MHz clock --> clock/(PSC+1) = 1MHz, matches useconds
	
	// Auto-reload (also 16b)
	TIM2->ARR = 2000-1;			// 1MHz clock (see above), period = 2ms --> ARR = clock*period - 1
	
	// Count direction
	COUNT_DIR(TIM2->CR1, COUNT_UP);
	
	// Enable interrupts
	SET_BITS(TIM2->DIER, TIM_DIER_UIE);
	
	// Enable TIM2 in NVIC
	NVIC_EnableIRQ(TIM2_IRQn);
	
	// Enable timer 1
	SET_BITS(TIM2->CR1, TIM_CR1_CEN);
	
} // End Heartbeat_Init


// Timer 2 IRQ (Heartbeat timer)
void TIM2_IRQHandler(void) {
	
	// Check if the timer has overflowed
	if((TIM2->SR & TIM_SR_UIF) != 0) {
		
		// Check for ping request
		if (pingFlag) {
			vportPrintf("\n\rp\n");
			pingFlag = 0;				// Clear
		} // End if
		
		// Check for LED flip request
		if(ledFlag) {
			Red_LED_Toggle();			// Not actually red but whatever
			ledFlag = 0;
		} // End if
		
	} // End if
	TIM2->SR &= ~TIM_SR_UIF; 			// Clear interrupt request to clear
	
} // End TIM2_IRQHandler
