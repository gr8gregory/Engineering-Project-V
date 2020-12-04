/*****************************************************************************
 * limit.c
 * 
 * Contains functions initialize 2 limit switches and deal with related IRQs
 * 
 * Caleb Hoeksema, Gregory Huras, Andrew Sammut
 * October 2020
 ****************************************************************************/

/* For now, we will use:
 * 		Left Limit: PB5
 * 		Right Limit: PB6
 */
 
// Header files
#include "limit.h"


// Global Variables
volatile uint8_t leftLimitFlag;		// Indicates left limit reached
volatile uint8_t rightLimitFlag; 	// Indicates right limit reached
uint8_t startup = 1;							// Indicates startup or not
uint8_t limitsFlipped = 0;				// Indicates if limts are reversed between HW and SW


// Init LimSws
void limit_Init(void) {
	
	// Start clock
	GPIO_clock_enable(LIMIT_CLK);
	// RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	
	// Set up left limit pin
	GPIOx_PIN_MODE(LIMIT_PORT, LEFT_LIM_PIN, MODER_IN);
	GPIOx_OP_TYPE( LIMIT_PORT, LEFT_LIM_PIN, OPEN_DR);
	GPIOx_PIN_SPEED(LIMIT_PORT, LEFT_LIM_PIN, HI_SPEED);
	
	// Set up right limit pin
	GPIOx_PIN_MODE(LIMIT_PORT, RIGHT_LIM_PIN, MODER_IN);
	GPIOx_OP_TYPE( LIMIT_PORT, RIGHT_LIM_PIN, OPEN_DR);
	GPIOx_PIN_SPEED(LIMIT_PORT, RIGHT_LIM_PIN, HI_SPEED);

	// FROM HERE ON, WE ASSUME THAT THE LIMIT SWITCHES ARE ON PINS 5 AND 6! WE ALSO ARE ASSUMING THE PORT IS GPIOB!
	
	// EXTI5,6 Interrupt Enable
	NVIC_EnableIRQ(EXTI9_5_IRQn);				// Pins 5-9 are on the same interrupt
	NVIC_SetPriority(EXTI9_5_IRQn, 0);	// Priorities are the same for now
	
	// Connect pins to EXTIx - pins 5 and 6, port C
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[1] &= ~SYSCFG_EXTICR2_EXTI5;     // SYSCFG external interrupt configuration registers
	SYSCFG->EXTICR[1] |=  SYSCFG_EXTICR2_EXTI5_PC;
	SYSCFG->EXTICR[1] &= ~SYSCFG_EXTICR2_EXTI6;     // SYSCFG external interrupt configuration registers
	SYSCFG->EXTICR[1] |=  SYSCFG_EXTICR2_EXTI6_PC;

	// Interrupt Mask Register
	EXTI->IMR |= EXTI_IMR_IM5;     // 0 = marked, 1 = not masked (i.e., enabled)
	EXTI->IMR |= EXTI_IMR_IM6;
	
	// Rising trigger selection (trigger on rising edge)
	EXTI->FTSR |= EXTI_FTSR_TR5;     // 0 = trigger disabled, 1 = trigger enabled
	EXTI->FTSR |= EXTI_FTSR_TR6;

} // End limit_Init()


// Limit IRQ
	// ASSUMES PIN NUMBER OF LIMIT SWITCHES------------------------------------------------
void EXTI9_5_IRQHandler(void) {
	
	uint8_t junk = 0;
	// Check EXT8
	if ((EXTI->PR & EXTI_PR_PIF5) == EXTI_PR_PIF5) {
		EXTI->PR = EXTI_PR_PIF5; 		// Clear the interrupt
		leftLimitFlag = 1;
		
		// Only true if there is a problem
		if (!startup) {
			leftLimitFlag = 0;
			stepHome();
			
		} // End if
	} // End if
	
	// Check EXT9
	if ((EXTI->PR & EXTI_PR_PIF6) == EXTI_PR_PIF6) {
		EXTI->PR = EXTI_PR_PIF6; 		// Clear the interrupt
		rightLimitFlag = 1;
		
		// Only true if there is a problem
		if (!startup) {
			rightLimitFlag = 0;
			stepHome();
			
		} // End if
	} // End if
	
	// Clear false interrupts
	junk = EXTI_PR_PIF5;
	junk = EXTI_PR_PIF6;
	
} // End EXTI9_5_IRQHandler()


