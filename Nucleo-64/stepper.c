/*****************************************************************************
 * stepper.c
 * 
 * Contains functions initialize a stepper motor and deal with related IRQs
 * 
 * Caleb Hoeksema, Gregory Huras, Andrew Sammut
 * October 2020
 ****************************************************************************/

/* For now, we will use:
 * 		Pin A: PC0
 * 		Pin B: PC1
 * 		Pin C: PC2
 * 		Pin D: PC3
 *
 * The step type is set by the global variable. We chose right to be the 
 *	 positive direction and left to be the negative direction.
 * THE STEPPER MOTOR IS UPDATED BASED ON TIMER 3.
 *
 * step = (stepCount_step/FULL_RIGHT_DEG) * degs
 * degs = (stepCount_step/FULL_RIGHT_DEG) * step
 */
 
 // Header files
#include "stepper.h"


// Global Variables
volatile uint32_t stepCurPos_step = 5000;				// Not sure how large this will get
volatile uint32_t stepGoToPos_step = 0;				// Same range as stepCurPos
uint32_t stepCount_step = 0xA000;									// Same range as stepCurPos
uint16_t overflow = 0;
extern volatile uint8_t leftLimitFlag;				// Indicates left limit reached
extern volatile uint8_t rightLimitFlag; 			// Indicates right limit reached
extern volatile uint8_t limitsFlipped;				// Indicates reversal between SW and HW
static const uint8_t halfStep[8] = {0x9, 0x8, 0xa, 0x2, 0x6, 0x4, 0x5, 0x1};		// Declared here, for use in the stepper ISR and homing
volatile uint8_t nextStep = 0;								// Offset into halfStep[]

static uint8_t stepType = FULL_STEP;

uint32_t getStep(void )
{
	return ((stepCurPos_step * FULL_RIGHT_DEG ) / (stepCount_step));
}


// Home stepper motor on initialization
void stepper_Init(void) {
	
	// Set up GPIO pins
	GPIO_clock_enable(STEP_CLK);												// Turn on port clock
	
	GPIOx_PIN_MODE(STEP_PORT, STEP_PIN_A, MODER_OUT);
	GPIOx_OP_TYPE( STEP_PORT, STEP_PIN_A, PUSH_PULL);
	GPIOx_PIN_PULL(STEP_PORT, STEP_PIN_A, PULL_UP);
	
	GPIOx_PIN_MODE(STEP_PORT, STEP_PIN_B, MODER_OUT);
	GPIOx_OP_TYPE( STEP_PORT, STEP_PIN_B, PUSH_PULL);
	GPIOx_PIN_PULL(STEP_PORT, STEP_PIN_B, PULL_UP);
	
	GPIOx_PIN_MODE(STEP_PORT, STEP_PIN_C, MODER_OUT);
	GPIOx_OP_TYPE( STEP_PORT, STEP_PIN_C, PUSH_PULL);
	GPIOx_PIN_PULL(STEP_PORT, STEP_PIN_C, PULL_UP);
	
	GPIOx_PIN_MODE(STEP_PORT, STEP_PIN_D, MODER_OUT);
	GPIOx_OP_TYPE( STEP_PORT, STEP_PIN_D, PUSH_PULL);
	GPIOx_PIN_PULL(STEP_PORT, STEP_PIN_D, PULL_UP);
	
	// Home the stepper motor
	//stepHome();
	step_clock_Init();

} // End stepper_Init()


void stepHome(void) {
	
	// Local Variables
	uint8_t takeStep = 0;
	
	// We don't want the stepper motor to move from here on
	stepGoToPos_step = stepCurPos_step = 0;
	
	// Step and check both flags while moving full left
	while((!leftLimitFlag) && (!rightLimitFlag)) {
		nextStep = (nextStep - stepType) & 0x7;		// Mask out any overflow over 8
		takeStep = halfStep[nextStep];						// Get desired bit pattern
		FORCE_BITS(STEP_PORT->ODR, 0xF, takeStep);
	}
	
	// Deal with flags and wiring
	if (leftLimitFlag)													// If properly wired
		leftLimitFlag = 0;
	
	else if (rightLimitFlag) {									// If wired backwards
		rightLimitFlag = 1;
		limitsFlipped = 1;
	} // End else if
	
	else {}
		// Something is really wrong if it gets here...
	
	// Finish homing the motor
	if (!limitsFlipped) {
		
		// Move  full right, checking for limit (we chose right to be the positive direction)
		while(!rightLimitFlag) {
			nextStep = (nextStep + stepType) & 0x7;		// Mask out any overflow over 8
			takeStep = halfStep[nextStep];						// Get desired bit pattern
			FORCE_BITS(STEP_PORT->ODR, 0xF, takeStep);
			
			if (stepCount_step >= UINT32_MAX) {
				overflow++;
				stepCount_step = 0;
			} // End if
			else
				stepCount_step++;
		}
		rightLimitFlag = 0;													// Reset the flag
	
	} // End if
	
	else {																				// We are calling left right and right left
		
		// Move  full right, checking for limit (we chose right to be the positive direction)
		while(!leftLimitFlag) {
			nextStep = (nextStep + stepType) & 0x7;		// Mask out any overflow over 8
			takeStep = halfStep[nextStep];						// Get desired bit pattern
			FORCE_BITS(STEP_PORT->ODR, 0xF, takeStep);
			
			if (stepCount_step >= UINT32_MAX) {
				overflow++;
				stepCount_step = 0;
			} // End if
			else
				stepCount_step++;
		}
		leftLimitFlag = 0;													// Reset the flag

	} // End else
	
	DisableInterrupts;													// Otherwise the interrupt will start moving the motor (maybe let the interrupt do this later?)
		stepCurPos_step = stepCount_step;						// Set position so we can move the motor
		
		while((stepCurPos_step) != (stepCount_step/2)) {					// Comparing DEGS, not STEPS
			nextStep = (nextStep - stepType) & 0x7;		// Mask out any overflow over 8
			takeStep = halfStep[nextStep];						// Get desired bit pattern
			FORCE_BITS(STEP_PORT->ODR, 0xF, takeStep);
			stepCurPos_step--;
		}
		
		// Set GoTo flags
		stepGoToPos_step = stepCurPos_step;					// So the motor doesn't immediately go back to wherever it was
	
	EnableInterrupts;														// Motor homed, system can resume control
	
} // End stepHome()


// Set up timer for checking motor (ISR)
static void step_clock_Init(void) {
	
	// Enable the clock-------------------------------------------
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;	// TIMER_CLOCK_ENABLE();
	
	// Disable Timer
	TIMx_ENABLE(STEP_TIM, TIMER_OFF);
	
	// Clock Prescale (16 bits - up to 65 535)
	STEP_TIM->PSC = 71;				//72MHz clock --> clock/(PSC+1) = 1MHz, matches useconds
	
	// Auto-reload (also 16b)
	STEP_TIM->ARR = 1000-1;		//1MHz clock (see above), period = 1ms --> ARR = clock*period - 1
	
	// Count direction
	TIMx_COUNT_DIR(STEP_TIM, TIM_CNT_UP);

	// Enable interrupts
	SET_BITS(STEP_TIM->DIER, TIM_DIER_UIE);
	
	// Enable TIM2 in NVIC-------------------------------------------
	NVIC_EnableIRQ(TIM3_IRQn);
	
	// Enable timer 1
	SET_BITS(STEP_TIM->CR1, TIM_CR1_CEN);
	
} // End STEP_CLOCK_Init()
	

// Check motor on timer rising edge
	// If not in the right spot (check global), move the motor
void TIM3_IRQHandler(void) {
	
	// Check if the timer has overflowed
	if((STEP_TIM->SR & TIM_SR_UIF) != 0) {
		
		// Do we need to move the stepper motor
			// Full left is 0, full right is max
		if (stepCurPos_step != stepGoToPos_step) {
			
			if (stepCurPos_step > stepGoToPos_step) {						// If current is gt goal, step left (-)
				nextStep = (nextStep - stepType) & 0x7;						// Mask out any overflow over 8
				FORCE_BITS(STEP_PORT->ODR, 0xF, halfStep[nextStep]);
				stepCurPos_step--;
			} // End if
			
			else {																							// If current is lt goal, step right (+)
				nextStep = (nextStep + stepType) & 0x7;						// Mask out any overflow over 8
				FORCE_BITS(STEP_PORT->ODR, 0xF, halfStep[nextStep]);
				stepCurPos_step++;
			} // End else
			
		} // End if
		
	} // End if
	STEP_TIM->SR &= ~TIM_SR_UIF; 			// Clear interrupt request to clear
	
} // End TIM2_IRQHandler

