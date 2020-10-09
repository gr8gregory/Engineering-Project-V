#include "LED.h"
#include "GPIO.h"


//******************************************************************************************
// User LEDs: 
//   LD4 Red = PB2    LD5 Green = PE8
// Note: The Green LED is yellow on my board.
//       PE8 is also the TIM1_CH1N for ADC Triggers.
//******************************************************************************************
void LED_Init(void){
	/* Enable GPIOs clock */ 	
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	// LD2 = PA5
	///////////////////////////////////////////////////////////////////////////////////////////////
	// GPIO Mode: Input(00, reset), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOA_PIN_MODE(LED_PIN, MODER_OUT);
	
	/*************************************************************/
	// GPIOA->MODER = ~(3U<<(2*2));  
	// GPIOA->MODER |= 1U<<(2*2);      //  Output(01)
	
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOA_PIN_SPEED(LED_PIN, HI_SPEED);
	// GPIOA->OSPEEDR &= ~(3U<<(2*2));
	// GPIOA->OSPEEDR |=   3U<<(2*2);  // High speed
	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	GPIOA_OP_TYPE(LED_PIN, PUSH_PULL);
	//GPIOB->OTYPER &= ~(1U<<2);       // Push-pull
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA_PIN_PULL(LED_PIN, PULL_NONE);
	//GPIOB->PUPDR   &= ~(3U<<(2*2));  // No pull-up, no pull-down
	
}

//******************************************************************************************
// Turn Red LED On
//******************************************************************************************
void Red_LED_On(void){
	GPIOA->ODR |= GPIO_ODR_5;
}

//******************************************************************************************
// Turn Red LED Off
//******************************************************************************************
void Red_LED_Off(void){
	GPIOA->ODR &= ~GPIO_ODR_5;
}

//******************************************************************************************
// Toggle Red LED 
//******************************************************************************************
void Red_LED_Toggle(void){
	GPIOA->ODR ^= GPIO_ODR_5;
}
