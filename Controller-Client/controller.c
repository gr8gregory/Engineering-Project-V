/* Gregs Controller Button Mapout
 * Button 0: A
 * Button 1: B
 * Button 2: C
 * Button 3: X
 * Button 4: Y
 * Button 5: Z
 * Button 6: L Trig
 * Button 7: R Trig
 ********* First column is location on controller, second column is direction. L L = Left joystick Left direction ************
 * Axis 0: L L
 * Axis 1: L U
 * Axis 2: Slider
 * Axis 3: R L
 * Axis 4: R U
 * Axis 5: D L
 * Axis 6: D U
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "controllerH.h"
#include "client.h"

#define JS_EVENT_BUTTON        0x01    /* button pressed/released */
#define JS_EVENT_AXIS          0x02    /* joystick moved */
#define JS_EVENT_INIT		0x80    /* initial state of device */

// Define which controller is hooked up For Program for different controller switch statement
#define GREG
//#define CALEB
//#define ANDREW

struct js_event {
	unsigned int time;	/* event timestamp in milliseconds */
	short value;   	/* value */
	unsigned char type;	/* event type */
	unsigned char number;	/* axis/button number */
};

int fd = 0;

char cmd;
unsigned short val;

int controller (void){
	
	int status = 0;
	// Check to see if the device can be opened
	int fd = open ("/dev/input/js0", O_RDONLY);
 
	if( fd < 0 ){ 
		printf("cannot open dev\n");
		return 1;
 	}
 	
	else{
		printf("opened success...:)\n");

	}
	
	while (1){
	
		struct js_event event;
		read( fd, &event, sizeof(event) ); // Program keeps getting stuck here
		if( event.type == JS_EVENT_BUTTON || event.type == JS_EVENT_AXIS ){
		
			if( event.type == JS_EVENT_BUTTON ){ 
				status = command( event.number, event.value, 0);
			}
			else{ 
				status = command(event.number, event.value, 1);	
			}
		}
		else{
 
			printf("Init Events\n");
		}
 
	} //- End while
	
	return 1;

}

int command (u_int16_t number, u_int16_t value, int Periferal){
	#ifdef GREG
	cmd = 0x0;
	if( Periferal == 1){
	
		switch(number){ // This switch is for the axis cases
			
			case 0:
				//printf( "axis#%x value:%x\n", number, value );
				
				cmd = 0x1;
				val = value;
				
				break;
				
			case 1:
				//printf( "axis#%x value:%x\n", number, value );
				cmd = 0x2;
				val = value;
				break;
				
			case 2:
				//printf( "axis#%x value:%x\n", number, value );
				cmd = 0x3;
				val = value;
				break;
				
			case 3:
				//printf( "axis#%x value:%x\n", number, value );
				cmd = 0x4;
				val = value;
				break;
				
			case 4:
				//printf( "axis#%x value:%x\n", number, value );
				cmd = 0x5;
				val = value;
				break;
				
			case 5:
				//printf( "axis#%x value:%x\n", number, value );
				cmd = 0x6;
				val = value;
				break;
				
			case 6:
				//printf( "axis#%x value:%x\n", number, value );
				cmd = 0x7;
				val = value;
				break;
				
			default:
				return 1;
		}	
	}
	else{
	
	switch(number){ // This switch is for the Button cases.
			
			case 0:
				//printf( "button#%d value:%d\n", number, value );
				cmd = 0x8;
				val = value;
				break;
				
			case 1:
				//printf( "button#%d value:%d\n", number, value );
				cmd = 0x9;
				val = value;
				break;
				
			case 2:
				//printf( "button#%d value:%d\n", number, value );
				cmd = 0xa;
				val = value;
				break;
				
			case 3:
				//printf( "button#%d value:%d\n", number, value );
				cmd = 0xb; 
				val = value;
				break;
				
			case 4:
				//printf( "button#%d value:%d\n", number, value );
				cmd = 0xc;
				val = value;
				break;
				
			case 5:
				//printf( "button#%d value:%d\n", number, value );
				cmd = 0xd;
				val = value;
				break;
				
			case 6:
				//printf( "button#%d value:%d\n", number, value );
				cmd = 0xe;
				val = value;
				break;
			case 7:
				//printf( "button#%d value:%d\n", number, value );
				cmd = 0xf;
				val = value;
				break;
			case 8:
				//printf( "button#%d value:%d\n", number, value );
				cmd = 0x10;
				val = value;
				break;
				
			default:
				return 1;
		}	
	
	}
	
	if (cmd != 0x0){
			printf("Command Recieved\n");
			sendCMD(cmd,val);
	}
	#endif
	
	#ifdef CALEB
	// Some switch statement paired with their controller
	#endif
	
	#ifdef ANDREW
	// Some switch statement paried with their controller
	#endif
	
	return 0;
}





