/*****************************************************************************
 * servo.c
 * 
 * Contains functions initialize a servomotor and deal with related IRQs
 * 
 * Caleb Hoeksema, Gregory Huras, Andrew Sammut
 * November 2020
 ****************************************************************************/

/* For now, we will use:
 * 		Pin PB0
 *
 * We chose up to be the positive direction and down to be the negative direction
 * THE SERVO IS RUNNING ON TIMER 1
 *
 * Full down: 0 deg
 * Full up: 180deg
 */
 
 // Header files
#include "servo.h"


// Global Variables
volatile uint32_t servoCurPos_deg = 0;				// Not sure how large this will get
volatile uint32_t servoCurPos_sec = 0;				// Same range as servoCurPos


// Home stepper motor on initialization
void servo_Init(void) {
	
	// Set up GPIO pins
	GPIO_clock_enable(SERVO_CLK);												// Turn on port clock
	
	GPIOx_PIN_MODE(SERVO_PORT, SERVO_PIN, MODER_AF);
	GPIOx_AF_MODE(SERVO_PORT, SERVO_PIN, SERVO_AF6);
	
	GPIOx_PIN_SPEED(SERVO_PORT, SERVO_PIN, LOW_SPEED);
	GPIOx_PIN_PULL(SERVO_PORT, SERVO_PIN, PULL_NONE);
	
	// Home the stepper motor
	servo_clock_Init();
	servoSet(90);

} // End servo_Init()


// Function to set the position of the servo
// Input is 16b in case we want to input from 0 1800 instead of from 0 to 180
void servoSet(uint16_t angle) {
	
	// Local Variables
	uint16_t pulse;
	
	// Convert from angle to time (10 is slope from graph)
	pulse = (angle * 10) + PULSE_MIN;
	
	// Write port
	TIM1->CCR2 = pulse;
	
	// Write servo_curpos_deg and servo_curpos_sec (ditch other vars)
	servoCurPos_deg = angle;
	servoCurPos_sec = pulse;

} // End servoSet()


// Set up timer for checking motor (ISR)
static void servo_clock_Init(void) {
	// ----------------------------------------------------------------------------------------
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;	// TIMER_CLOCK_ENABLE();
	
	// Disable Timer
	TIMx_ENABLE(SERVO_TIM, TIMER_OFF);
	// CLR_BITS(SERVO_TIM->CR1, TIM_CR1_CEN);
	
	// Count direction
	// TIM1->CR1 &= COUNT_UP;
	TIMx_COUNT_DIR(SERVO_TIM, TIM_CNT_UP);
	//COUNT_DIR(SERVO_TIM->CR1, COUNT_UP);
	
	// Clock Prescale (16 bits - up to 65 535)
	SERVO_TIM->PSC = 71;					//72MHz clock --> clock/(PSC+1) = 1MHz, matches useconds
	
	// Auto-reload (also 16b)
	SERVO_TIM->ARR = 20000-1;			//1MHz clock (from prescaler), period = 20ms --> ARR = clock*period - 1
	
	// NON-GENERIC TO NEXT MARKER -----------------------------------------------
		// Set mode (PWM is 0110) (varies with channel)
		FORCE_BITS(SERVO_TIM->CCMR1, TIM_CCMR1_OC2M_Msk, (TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2));
		
		// Preload enable (varies with channel)
		SERVO_TIM->CCMR1 |= TIM_CCMR1_OC2PE;
		
		// Output Polarity (varies with channel)
		SERVO_TIM->CCER &= ~TIM_CCER_CC2NP;
		
		// Enable channel 2 output (varies with channel)
		SERVO_TIM->CCER |= TIM_CCER_CC2NE;
	// --------------------------------------------------------------------------
	
	// Main OP enable
	TIMx_OP_ENABLE(SERVO_TIM, TIM_OP_ON);
	// OP_ENABLE(OP_ON); // FORCE_BITS(TIM1->BDTR, TIM_BDTR_MOE, (mode))
	
	// Enable Timer
	TIMx_ENABLE(SERVO_TIM, TIMER_ON);
	// SET_BITS(TIM1->CR1, TIM_CR1_CEN);		// SET_BITS(TIM2->CR1, TIM_CR1_CEN);
	
} // End STEP_CLOCK_Init
	

