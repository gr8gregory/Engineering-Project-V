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
	GPIOA_PIN_MODE(LED_PIN, MODER_OUT);
	
	// GPIO Speed
	GPIOA_PIN_SPEED(LED_PIN, HI_SPEED);
	
	// GPIO Output Type 
	GPIOA_OP_TYPE(LED_PIN, PUSH_PULL);
	
	// GPIO Push-Pull
	GPIOA_PIN_PULL(LED_PIN, PULL_NONE);
	
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
