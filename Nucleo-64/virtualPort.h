/*****************************************************************************
 * virtualPort.h
 *
 * Header file for running a virtual serial port for use with a terminal
 *		emulator
 *
 * Caleb Hoeksema, Gregory Huras, Anrew Sammut
 * October 2020
 ****************************************************************************/


#ifndef		__VIRTUALPORT_H
#define		__VIRTUALPORT_H

// Header files
#include 	"GPIO.h"
#include 	<stdarg.h>
#include 	<stdio.h>
#include	"USART.h"


// Constants
#define		VP_CLEAR						vportPrintf("\033[2J");			// Clear screen
#define		VP_RESET						vportPrintf("\033[0;0H"); 	// Cursor to 0,0
#define		VP_NEW_LINE					vportPrintf("\033[1B")			// Cursor down 1 line

// Pins: Virtual port (USART2)
#define 	VPORT_TX_PIN				2UL
#define 	VPORT_RX_PIN				3UL

// Alternate Function
#define		VPORT_AF7						0x7UL

// Ports
#define		VPORT_CLK		RCC_AHBENR_GPIOAEN
#define 	VIRT_PORT		GPIOA

// Function prototypes
void virtualPortInit(void);
void vportPrintf(char *fmt, ...);
static void vportPuts(char *str);
static void vportPutc(char c);
void USART2_IRQHandler(void);
void vportMenuPrint(void);
void clearInput(void);


#endif
