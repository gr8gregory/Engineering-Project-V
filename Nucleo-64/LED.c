/*****************************************************************************
 * LED.c
 * 
 * Contains functions to initialize and change the state of an LED
 * 
 * Caleb Hoeksema, Gregory Huras, Andrew Sammut, Bill Stefanuk
 * October 2020
 ****************************************************************************/
 
// Header files
#include "LED.h"
#include "GPIO.h"


// Function to initialize the LED 
	// User LED is LD2 (Green) on PA5
	// The functions imply a red LED but it is actually Green on the board
void LED_Init(void){
	
	/* Enable GPIOs clock */ 	
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	
	// GPIO Mode
	GPIOx_PIN_MODE(VIRT_PORT, LED_PIN, MODER_OUT);
	
	// GPIO Speed
	GPIOx_PIN_SPEED(GPIOA, LED_PIN, HI_SPEED);
	
	// GPIO Output Type 
	GPIOx_OP_TYPE(GPIOA, LED_PIN, PUSH_PULL);
	
	// GPIO Push-Pull
	GPIOx_PIN_PULL(GPIOA, LED_PIN, PULL_NONE);
	
} // End LED_Init


// Fucntion to turn on LED
void Red_LED_On(void){
	
	GPIOA->ODR |= GPIO_ODR_5;
	
} // End Red_LED_On


// Function to turn off LED
void Red_LED_Off(void){
	
	GPIOA->ODR &= ~GPIO_ODR_5;
	
} // End Red_LED_Off


// Funciton to toggle LED state
void Red_LED_Toggle(void){
	
	GPIOA->ODR ^= GPIO_ODR_5;

} // End Red_LED_Toggle
