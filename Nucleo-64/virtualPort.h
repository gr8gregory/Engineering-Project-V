/*****************************************************************************
 * virtualPort.h
 *
 * Header file for running a virtual serial port for use with a terminal
 *		emulator
 *
 * Caleb Hoeksema
 * April 2020
 ****************************************************************************/


#ifndef		__VIRTUALPORT_H
#define		__VIRTUALPORT_H

// Header files
#include 	"GPIO.h"
#include 	<stdarg.h>
#include 	<stdio.h>
#include 	"stm32f303xe.h"
#include 	"SysClock.h"
#include	"USART.h"
#include 	"utils.h"


// Constants
#define		VP_CLEAR						vportPrintf("\033[2J");		// Clear screen
#define		VP_RESET						vportPrintf("\033[0;0H"); // Cursor to 0,0
#define		VP_NEW_LINE					vportPrintf("\033[1B")		// Cursor down 1 line

// Pins: Virtual port (USART2)
#define 	VPORT_TX_PIN				2UL
#define 	VPORT_RX_PIN				3UL

// Function prototypes
void virtualPortInit(void);
static void gpioConfig(void);
static void vportPutc(char c);
static void vportPuts(char *str);
void vportPrintf(char *fmt, ...);
char vportGetc(void);


#endif
