/*****************************************************************************
 * dcmotor.c
 * 
 * Contains functions initialize a pair of DC motors and deal with related actions
 * 
 * Caleb Hoeksema, Gregory Huras, Andrew Sammut
 * December 2020
 ****************************************************************************/

// Header file
#include "dcmotor.h"


// Global Variables
volatile char dirL;
volatile char dirR;


// Function to set up DC Motor drive (PWM) GPIOs
void DC_DRV_Init(void) {
	
	// Start clock
	GPIO_clock_enable(DC_DRV_CLK);
	
	// Set up pins
	GPIOx_PIN_MODE(	DC_DRV_PORT, DC_DRV_L_PIN, MODER_AF);
	GPIOx_PIN_MODE(	DC_DRV_PORT, DC_DRV_R_PIN, MODER_AF);
	
	// Need to use "AFH" variant since pin numbers are greater than 8
	GPIOx_AFH_MODE(	DC_DRV_PORT, DC_DRV_L_PIN, DC_DRV_AF4);
	GPIOx_AFH_MODE(	DC_DRV_PORT, DC_DRV_R_PIN, DC_DRV_AF4);
	
	GPIOx_PIN_SPEED(DC_DRV_PORT, DC_DRV_L_PIN, HI_SPEED);
	GPIOx_PIN_SPEED(DC_DRV_PORT, DC_DRV_R_PIN, HI_SPEED);
	
	GPIOx_PIN_PULL(	DC_DRV_PORT, DC_DRV_L_PIN, PULL_NONE);
	GPIOx_PIN_PULL(	DC_DRV_PORT, DC_DRV_R_PIN, PULL_NONE);
	
	// Need to set up the clock/timer here (Timer 8, Channels 1N and 2N)
	DC_DRV_clock_Init();
	// Shouldn't need any kind of interrupt -> like the servo, you just write
		// the CCRx register and it does the rest
	
	// Choose a random speed and direction for now 
	dirL = FWD;
	dirR = BWD;
	dcMotorSet(10,10);
	
} // End DC_DRV_Init()


// Function to set up timer for speed updates
static void DC_DRV_clock_Init(void) {
	
	// Start clock (NON-GENERIC!)
	RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;
	
	// Disable timer
	TIMx_ENABLE(DC_DRV_TIM, TIMER_OFF);
	
	// Count up
	TIMx_COUNT_DIR(DC_DRV_TIM, TIM_CNT_UP);
	
	// Match to useconds [see servo.c/servo_clock_Init()]
	DC_DRV_TIM->PSC = 71;
	
	// Set the period to 30ms
	DC_DRV_TIM->ARR = 30000-1;
	
	/*------------------------------------------*/
	// Set mode to PWM
	FORCE_BITS(DC_DRV_TIM->CCMR1, TIM_CCMR1_OC1M_Msk, (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2));
	FORCE_BITS(DC_DRV_TIM->CCMR1, TIM_CCMR1_OC2M_Msk, (TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2));
		
	// Preload enable (varies with channel)
	DC_DRV_TIM->CCMR1 |= TIM_CCMR1_OC1PE;	
	DC_DRV_TIM->CCMR1 |= TIM_CCMR1_OC2PE;
	
	// Output Polarity (varies with channel)
	DC_DRV_TIM->CCER &= ~TIM_CCER_CC1NP;
	DC_DRV_TIM->CCER &= ~TIM_CCER_CC2NP;
	
	// Enable channel 2 output (varies with channel)
	DC_DRV_TIM->CCER |= TIM_CCER_CC1NE;
	DC_DRV_TIM->CCER |= TIM_CCER_CC2NE;
	/*------------------------------------------*/
	
	TIMx_OP_ENABLE(DC_DRV_TIM, TIM_OP_ON);
	
	TIMx_ENABLE(DC_DRV_TIM, TIMER_ON);
	
} // End DC_DRV_clock_Init()


// Function to set motor speed
void dcMotorSet(uint16_t left, uint16_t right) {
	
	// Local Variables
	uint16_t pulseL;
	uint16_t pulseR;
	
	// PWM goes from 0 to 30000 which is 0 to 30 ms
	// For now, speeds go from 0 to 3000 so the slope is 10
	if (left != SAME_SPEED) {
		pulseL = (left * 10);
		DC_DRV_TIM->CCR1 = pulseL;
	} // End if
	
	if (right != SAME_SPEED) {
		pulseR = (right * 10);
		DC_DRV_TIM->CCR2 = pulseR;
	} // End if

} // End dcMotorSet()


/*** Direction Functions ***/
// Directions actually set by the heartbeat timer

// Function to set up DC Motor direction GPIOs
void DC_DIR_Init(void) {
	
	// Start clock
	GPIO_clock_enable(DC_DIR_L_CLK);
	GPIO_clock_enable(DC_DIR_R_CLK);
	
	// Set up pins
	GPIOx_PIN_MODE(	DC_DIR_L_PORT, DC_DIR_FL_PIN, MODER_OUT);
	GPIOx_PIN_MODE(	DC_DIR_L_PORT, DC_DIR_BL_PIN, MODER_OUT);
	GPIOx_PIN_MODE(	DC_DIR_R_PORT, DC_DIR_FR_PIN, MODER_OUT);
	GPIOx_PIN_MODE(	DC_DIR_R_PORT, DC_DIR_BR_PIN, MODER_OUT);
	
	GPIOx_OP_TYPE(	DC_DIR_L_PORT, DC_DIR_FL_PIN, PUSH_PULL);
	GPIOx_OP_TYPE(	DC_DIR_L_PORT, DC_DIR_BL_PIN, PUSH_PULL);
	GPIOx_OP_TYPE(	DC_DIR_R_PORT, DC_DIR_FR_PIN, PUSH_PULL);
	GPIOx_OP_TYPE(	DC_DIR_R_PORT, DC_DIR_BR_PIN, PUSH_PULL);
	
	GPIOx_PIN_PULL(	DC_DIR_L_PORT, DC_DIR_FL_PIN, PULL_UP);
	GPIOx_PIN_PULL(	DC_DIR_L_PORT, DC_DIR_BL_PIN, PULL_UP);
	GPIOx_PIN_PULL(	DC_DIR_R_PORT, DC_DIR_FR_PIN, PULL_UP);
	GPIOx_PIN_PULL(	DC_DIR_R_PORT, DC_DIR_BR_PIN, PULL_UP);
	
	// Set up Interrupts (roll into the speed interrupt?)
	
} // End DC_DIR_Init()
