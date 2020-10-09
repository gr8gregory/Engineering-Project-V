/*****************************************************************************
 * utils.h
 * 
 * Header file containing various useful functions
 * 
 * Caleb Hoeksema, Gregory Huras, Andrew Sammut
 * October 2020
 ****************************************************************************/
 
 
#ifndef _UTILS_H
#define _UTILS_H

#include "stm32f303xe.h"

 
// Bit twiddling macros
#define	LOW_BYTE(arg)					((arg) & 0xFF)

#define	SET_BITS(port, mask)			((port) |= (mask))
#define	CLR_BITS(port, mask)			((port) &= ~(mask))
#define	FLIP_BITS(port, mask)			((port) ^= (mask))

#define	FORCE_BITS(port, mask, value)	(port) = ((port) & ~(mask)) | ((value) & (mask))

#define	LO_NYBBLE(val)					((val) & 0x0F)
#define	HI_NYBBLE(val)					(((val) >> 4) & 0x0F)

// LCD Macros
#define LCD_MOVE_CURSOR		(0x80)		// Command to write an address to DDRAM
#define LCD_NEW_LINE			(0x28)		// 0x28=40d, the start of the new line


// Function Prototypes

// LCD Functions
void LCDinit(void);
static void LCDcmd(uint8_t thisByte);
static void LCDdata(uint8_t thisByte);
void LCDputc(unsigned char thisChar);
void LCDputs(char *thisString);
void LCDprintf(char *fmt, ...);

// General Functions
void Delay_ms(uint32_t delayTime);

#endif
