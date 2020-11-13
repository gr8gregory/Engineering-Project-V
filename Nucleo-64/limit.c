/*****************************************************************************
 * limit.c
 * 
 * Contains functions initialize 2 limit switches and deal with related IRQs
 * 
 * Caleb Hoeksema, Gregory Huras, Andrew Sammut
 * October 2020
 ****************************************************************************/

/* For now, we will use:
 * 		Left Limit: PA8
 * 		Right Limit: PA9
 */
 
// Header files
#include "limit.h"


// Global Variables
volatile uint8_t leftLimitFlag;		// Indicates left limit reached
volatile uint8_t rightLimitFlag; 	// Indicates right limit reached
uint8_t startup = 1;							// Indicates startup or not


// Init LimSws
void limit_Init(void) {
	
	// Start clock
	GPIO_clock_enable(LIMIT_CLK);
	
	// Set up left limit pin
	GPIOx_PIN_MODE(LIMIT_PORT, LEFT_LIM_PIN, MODER_IN);
	GPIOx_OP_TYPE( LIMIT_PORT, LEFT_LIM_PIN, PUSH_PULL);
	GPIOx_PIN_PULL(LIMIT_PORT, LEFT_LIM_PIN, PULL_DOWN);
	
	// Set up right limit pin
	GPIOx_PIN_MODE(LIMIT_PORT, RIGHT_LIM_PIN, MODER_IN);
	GPIOx_OP_TYPE( LIMIT_PORT, RIGHT_LIM_PIN, PUSH_PULL);
	GPIOx_PIN_PULL(LIMIT_PORT, RIGHT_LIM_PIN, PULL_DOWN);
	
	// FROM HERE ON, WE ASSUME THAT THE LIMIT SWITCHES ARE ON PINS 8 AND 9! WE ALSO ARE ASSUMING THE PORT IS GPIOA!
	
	// EXTI8,9 Interrupt Enable
	NVIC_EnableIRQ(EXTI9_5_IRQn);				// Pins 5-9 are on the same interrupt
	NVIC_SetPriority(EXTI9_5_IRQn, 0);	// Priorities are the same for now
	
	// Connect pins to EXTIx - pins 8 and 9, port a
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR3_EXTI8;     // SYSCFG external interrupt configuration registers
	SYSCFG->EXTICR[0] |=  SYSCFG_EXTICR3_EXTI8_PA;
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR3_EXTI9;     // SYSCFG external interrupt configuration registers
	SYSCFG->EXTICR[0] |=  SYSCFG_EXTICR3_EXTI9_PA;

	// Interrupt Mask Register
	EXTI->IMR |= EXTI_IMR_IM8;     // 0 = marked, 1 = not masked (i.e., enabled)
	EXTI->IMR |= EXTI_IMR_IM9;
	
	// Rising trigger selection (trigger on rising edge)
	EXTI->RTSR |= EXTI_RTSR_RT8;     // 0 = trigger disabled, 1 = trigger enabled
	EXTI->RTSR |= EXTI_RTSR_RT9;

} // End limit_Init()


// Limit IRQ
	// ASSUMES PIN NUMBER OF LIMIT SWITCHES------------------------------------------------
void EXTI9_5_IRQHandler(void) {
	
	// Check EXT8
	if ((EXTI->PR & EXTI_PR_PIF8) == EXTI_PR_PIF8) {
		EXTI->PR = EXTI_PR_PIF8; 		// Clear the interrupt
		leftLimitFlag = 1;
		
		// Only true if there is a problem
		if (!startup) {
			leftLimitFlag = 0;
			stepHome();
			
		} // End if
	} // End if
	
	// Check EXT9
	if ((EXTI->PR & EXTI_PR_PIF9) == EXTI_PR_PIF9) {
		EXTI->PR = EXTI_PR_PIF9; 		// Clear the interrupt
		rightLimitFlag = 1;
		
		// Only true if there is a problem
		if (!startup) {
			rightLimitFlag = 0;
			stepHome();
			
		} // End if
	} // End if
	
} // End EXTI8_IRQHandler


