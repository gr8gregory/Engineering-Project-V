/*
 * 	controller.c
 * 		Read the value from the joystick hooked up to personal machine in a Virtual Machine enviroment
 *		Send the read value to the client to be sent off to the server.	
 *
 * 	Author: Gregory Huras,
 *	
 *	Sources for controller Code: https://kusemanohar.info/2015/12/19/programming-with-joystick-on-linux/ 
 */

/* Header Files */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "controllerH.h"
#include "client.h"

/* Macros For Reading Controller */
#define JS_EVENT_BUTTON        0x01    /* button pressed/released */
#define JS_EVENT_AXIS          0x02    /* joystick moved */
#define JS_EVENT_INIT		0x80    /* initial state of device */


/* Joy Stick Event strucuct */
struct js_event {
	unsigned int time;	/* event timestamp in milliseconds */
	unsigned short value;  /* value */
	unsigned char type;	/* event type */
	unsigned char number;	/* axis/button number */
};

/* Assign button map and Value from the Controller */
unsigned char cmd;
unsigned short val;

/* Controller Function
 * Opens up the controller device, reads the event and then send the value
 * to the Client.c program to send the informtaion to the Lab computer 
 */
int controller (void){
	
	int status = 0;
	// Check to see if the device can be opened
	int fd = open ("/dev/input/js2", O_RDONLY);
 	
 	/* Check if the controller can be opened */
	if( fd < 0 ){ 
		printf("cannot open dev\n");
		return 1;
 	}
 	
	else{
		printf("opened success...:)\n");
	}
	
	/* Create and Instance of the Controller struct */
	struct js_event event;
	
	/* Enter a while loop to constantly read the values on the controller */
	while (1){
		
		/* Read the controller */
		read( fd, &event, sizeof(event) );
		
		/* If the type of the event that is read corresponds to a button being pressed or axis being moved, 
		 * then read that value and event. 
		 */
		 
		if( event.type == JS_EVENT_BUTTON || event.type == JS_EVENT_AXIS ){
			
			// If the button is pressed
			
			
			if( event.type == JS_EVENT_BUTTON ){ 
				
				/* Send the event number (corresponds to which button or axis is being pressed) and send the read value and a flag indicating its a button being read
				 *
				 */
				status = command( event.number, event.value, 0);
			}
			// If the axis is pressed
			
			else{ 
				/* Send the event number (corresponds to which button or axis is being pressed) and send the read value and a flag indicating its an axis being read
				 *
				 */
				if((event.value > 0x4000) && (event.value < 0xC000)){
					status = command(event.number, event.value, 1);	
				}
			}
		}
		
		// If a button isnt been pressed, the controller is initializing.
		else{
 
			printf("Init Events\n");
		}
 
	} //- End while
	
	return 1;

}


/* Gregs Controller Button Mapout
 *
 ********* First column is location on controller, second column is direction. L L = Left joystick Left direction ************
 *
 *			Assigned Hex Value
 *
 * Axis 0: L L			0x1
 * Axis 1: L U			0x2
 * Axis 2: Slider		0x3
 * Axis 3: R L			0x4
 * Axis 4: R U			0x5
 * Axis 5: D L			0x6
 * Axis 6: D U			0x7
 *
 * Button 0: A			0x8
 * Button 1: B			0x9
 * Button 2: C			0xa
 * Button 3: X			0xb
 * Button 4: Y			0xc
 * Button 5: Z			0xd
 * Button 6: L Trig		0xe
 * Button 7: R Trig		0xf
 * Button 8: Save Button	0x10
 * 
 *
 */

/* 
 * Take teh read value, assign its corresponding hex value and send the information off to be sent to the lab computer
 */



// Update: Add Homeing for Servo And stepper, and 360 to right trigger, and potentially serial break
int command (u_int16_t number, u_int16_t value, int Periferal){
	

	// Set the cmd Char to 0
	cmd = 0x0;
	
	// if the axis flag has been set
	if( Periferal == 1){	
		
		// reading the number value (Corresponds to which axis or button has been pressed
		// 6 different axis's to read from
		switch(number){ 
			
			/*case 0:
				cmd = 0x1;
				val = value;
				
				break;
			*/
				
			case 1: // Left stick Y direction
				cmd = 0x2;
				val = value;
				break;
				
			/*case 2:
				cmd = 0x3;
				val = value;
				break;
			*/	
			
			case 3: // Right stick X direction
				cmd = 0x4;
				val = value;
				break;
				
			/*case 4:
				cmd = 0x5;
				val = value;
				break;
			*/
				
			case 5:
				cmd = 0x6;
				val = value;
				break;
				
			case 6:
				cmd = 0x7;
				val = value;
				break;
				
			default:
				return 1;
		}	
	}
	// else the button flag has been set
	else{
	
	// reading the number value (Corresponds to which axis or button has been pressed
	// 8 different axis's to read from
	switch(number){ 
			
			case 0:
				cmd = 0x8;
				val = value;
				break;
				
			case 1:
				cmd = 0x9;
				val = value;
				break;
				
			case 2:
				cmd = 0xa;
				val = value;
				break;
				
			case 3:
				cmd = 0xb; 

				val = value;
				break;
				
			case 4:
				cmd = 0xc;
				val = value;
				break;
				
			case 5:
				cmd = 0xd;
				val = value;
				break;
				
			case 6:
				cmd = 0xe;
				val = value;
				break;
			case 7:
				cmd = 0xf;
				val = value;
				break;
			case 8:
				cmd = 0x10;
				val = value;
				break;
				
			default:
				return 1;
		}	
	
	}
	
	/*
	 * If data has been read from a button or an axis, then send that information to the client.
	 */
	if (cmd != 0x0){
			sendCMD(cmd,val);
	}
	
	
	return 0;
}