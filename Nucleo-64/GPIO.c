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
void GPIO_CLOCK_ENABLE(volatile uint32_t port) {
	
	// Enable GPIO port clock
	RCC->AHBENR |= port;

} // End GPIO_CLOCK_ENABLE


// Set up GPIOA with VPORT_RX_PIN and VPORT_TX_PIN for terminal input and echo
void GPIOA_Init(void) {
	
	GPIO_CLOCK_ENABLE(VPORT_CLK);
	
	// Set pin mode (AF7 - USART2)
	GPIOx_PIN_MODE(VIRT_PORT, VPORT_TX_PIN, MODER_AF);
	GPIOx_PIN_MODE(VIRT_PORT, VPORT_RX_PIN, MODER_AF);
	GPIOA->AFR[0] &= ~(0x3UL)<<(VPORT_RX_PIN *4);
	GPIOA->AFR[0] &= ~(0x3UL)<<(VPORT_TX_PIN *4);
	GPIOA->AFR[0] |= 7UL<<(VPORT_RX_PIN *4);
	GPIOA->AFR[0] |= 7UL<<(VPORT_TX_PIN *4);
	
	// Set pin speed
	GPIOx_PIN_SPEED(VIRT_PORT, VPORT_TX_PIN, HI_SPEED);
	GPIOx_PIN_SPEED(VIRT_PORT, VPORT_RX_PIN, HI_SPEED);
	
	// Set push pull
	GPIOx_PIN_PULL(VIRT_PORT, VPORT_TX_PIN, PULL_UP);
	GPIOx_PIN_PULL(VIRT_PORT, VPORT_RX_PIN, PULL_UP);
	
	// Set output type
	GPIOx_PIN_DRV_TYPE(VIRT_PORT, VPORT_TX_PIN, PUSH_PULL);
	GPIOx_PIN_DRV_TYPE(VIRT_PORT, VPORT_RX_PIN, PUSH_PULL);
	
} // End GPIOA_Init


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
  
} // End Delay_ms
