/*****************************************************************************
 * GPIO.c
 * 
 * Contains code to initialize a GPIO port
 * 
 * Caleb Hoeksema, Gregory Huras, Andrew Sammut
 * October 2020
 ****************************************************************************/

// Header files
#include "GPIO.h"


// Initalize the GPIO Clocks
// Don't really like this function but oh well
void GPIO_clock_enable(volatile uint32_t port) {
	
	// Enable GPIO port clock
	RCC->AHBENR |= port;

} // End GPIO_CLOCK_ENABLE()


// Delay function using milliseconds
void Delay_ms(uint32_t msec){
  
	SysTick->CTRL = 0;            										// Disable SysTick
	SysTick->LOAD = (80000000UL / 1000UL) * msec;  		// Set reload register
	SysTick->VAL = 0;             										// Reset the SysTick counter value

	// Select clock: 1 = processor clock, 0 = external clock
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;			// Disable SysTick interrupt, 1 = Enable, 0 = Disable
																									
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;				// Enable SysTick
																									
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;					// Wait for timeout
																									
	while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
	
	// Disable SysTick until next time
	SysTick->CTRL = 0; 
  
} // End Delay_ms()
