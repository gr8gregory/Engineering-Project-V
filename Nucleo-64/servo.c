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
	SERVO_PORT->AFR[0] &= ~(0x3UL)<<(SERVO_PIN *4); // they are grouped in 4s
	SERVO_PORT->AFR[0] |= 6UL<<(SERVO_PIN *4);			// AF6 is timer 1
	
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
	
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;	// TIMER_CLOCK_ENABLE();
	
	// Disable Timer
	CLR_BITS(TIM1->CR1, TIM_CR1_CEN);
	
	// Count direction
	// TIM1->CR1 &= COUNT_UP;
	COUNT_DIR(TIM1->CR1, COUNT_UP);			// COUNT_DIR(TIM2->CR1, COUNT_UP); FORCE_BITS((timer), TIM_CR1_DIR_Pos, (dir))
	
	// Clock Prescale (16 bits - up to 65 535)
	TIM1->PSC = 71;					//72MHz clock --> clock/(PSC+1) = 1MHz, matches useconds
	
	// Auto-reload (also 16b)
	TIM1->ARR = 20000-1;			//1MHz clock (from prescaler), period = 20ms --> ARR = clock*period - 1
	
	// Set mode
	FORCE_BITS(TIM1->CCMR1, TIM_CCMR1_OC2M_Msk, (TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2));
	// TIM1_MODE(MODE_PWM); // FORCE_BITS(TIM1->CCMR1, TIM_CCMR1_OC1M, (mode))
	
	// Preload enable
	/*TIM1_PRELOAD(PRELOAD_ON);*/ // FORCE_BITS(TIM1->CCMR1, TIM_CCMR1_OC2PE, (mode))
	TIM1->CCMR1 |= TIM_CCMR1_OC2PE;
	
	// Output Polarity
	// TIM1_POLAR(ACT_HI); FORCE_BITS(TIM1->CCER, TIM_CCER_CC2NP, ACT_HI);
	TIM1->CCER &= ~TIM_CCER_CC2NP;
	
	// Enable channel 2 output (channel attached to PB0) ------------------------------------------------------------------------ NON-GENERIC!
	TIM1->CCER |= TIM_CCER_CC2NE;
	
	// Main OP enable
	OP_ENABLE(OP_ON); // FORCE_BITS(TIM1->BDTR, TIM_BDTR_MOE, (mode))
	
	/* Don't need interrupts since the servo is just "set" and it goes
	// Enable interrupts
	SET_BITS(TIM3->DIER, TIM_DIER_UIE);	// SET_BITS(TIM2->DIER, TIM_DIER_UIE);
	
	// Enable TIM2 in NVIC
	NVIC_EnableIRQ(TIM3_IRQn);					// NVIC_EnableIRQ(TIM2_IRQn);
	*/
	
	// Enable timer 1
	SET_BITS(TIM1->CR1, TIM_CR1_CEN);		// SET_BITS(TIM2->CR1, TIM_CR1_CEN);
	
} // End STEP_CLOCK_Init
	

