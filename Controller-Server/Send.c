#include "server-ip.h"

int sendPKG (unsigned char *buf, int len);


/* Gregs Controller Button Mapout
 *
 ********* First column is location on controller, second column is direction. L L = Left joystick Left direction ************
 *
 *			Assigned Hex Value
 *
 * Axis 3: R x			0x4 -> DC motor (Turn left or right) Spin wheels in opposite directon for this effect
 * Axis 4: R y			0x5 -> DC motor (Drive straight or back) Spin wheels in same direction.
 * Axis 5: D x			0x6 -> Stepper? (left to right) -> Press button down and the stepper will move at a set speed
 * Axis 6: D y			0x7 -> Servo    (up or Down)    -> Press button down and the servo will move up or down at a set speed.
 *
 * Button 6: L Trig		0xe -> ping
 * Button 7: R Trig		0xf -> LED
 * 
 *
 */



 /* input buf convention:
  *
  * -> { { Hex Label }, { hex upper-nibble value }, { hex lower-nibble value } }
  *
  * buf[0] = Hex label
  * buf[1] = hex upper-nibble value
  * buf[2] = hex lower-nibble value
  *
  */


/* DC motor PKG convention:
 *
 * pkg[0] = Operating Mode label
 * pkg[1] = Direction label
 * pkg[2] = space
 * pkg[3] = upper nibble of input value
 * pkg[4] = lower nibble of input value
 * pkg[5] = Null Termiantor
 *
 */

/* Ping / Flip command PKG convention:
 *
 * pkg[0] = Command lable -> either 'F' or 'P'
 * pkg[1] = Null terminator
 *
 */

/* Stepper Motor PKG convention:
 *
 * 
 *
 */

/* Servo PKG convention:
 *
 * 
 *
 */

int serverOutput (unsigned char* buf){
	
	
	unsigned short conversion = 0;
	unsigned short val = 0;
	unsigned char pkg[6];
	
	val |= (buf[1] << 8); 	// Upper Nibble
	val |= (buf[2]);	// Lower Nibble
		
	
	switch(buf[0]){
	
		case 0x4: // DC motor x
			
		
			pkg[0] = 'D';
			
			if(val < 0x8000){
				pkg[1] = 'R';
				pkg[2] = ' ';
				pkg[3] = buf[1];
				pkg[4] = buf[2];
				pkg[5] = '\0';
			}
			else{
				pkg[1] = 'L';
				pkg[2] = ' ';

				conversion = 0xFFFF - val; // Convert the value so that it will read 0 - 7FFF
				pkg[3] = (conversion >> 8) & 0xff;
				pkg[4] = conversion & 0xff;
				
				pkg[5] = '\0';
			}
			
			sendPKG(pkg, strlen(pkg));
			//printf("\n*** %s, %x%x ***\n", pkg, pkg[3], pkg[4]);
			
			break;
		case 0x2: // DC motor y
			
			pkg[0] = 'D';
			
			if(val < 0x8000){
				pkg[1] = 'B';
				pkg[2] = ' ';
				pkg[3] = buf[1];
				pkg[4] = buf[2];
				pkg[5] = '\0';
			}
			else{
				pkg[1] = 'B';
				pkg[2] = ' ';

				conversion = 0xFFFF - val; // Convert the value so that it will read 0 - 7FFF
				pkg[3] = (conversion >> 8) & 0xff;
				pkg[4] = conversion & 0xff;
				
				pkg[5] = '\0';
			}
			
			sendPKG(pkg, strlen(pkg));
			//printf("\n***%s, %x%x***\n", pkg, pkg[3], pkg[4]);
			
			break;
		case 0x6: // Stepper 
			if (val == 0x8001){
			printf("true\n");
				pkg[0] = 'S';
				pkg[1] = 'T';
				pkg[2] = ' ';
				pkg[3] = 'L';
				pkg[4] = '\0';
			}

			else{
				pkg[0] = 'S';
				pkg[1] = 'T';
				pkg[2] = ' ';
				pkg[3] = 'R';
				pkg[4] = '\0';
			}
			sendPKG(pkg, strlen(pkg));

			break;
		case 0x7: // Servo
			if (val == 0x8001){
				pkg[0] = 'S';
				pkg[1] = 'V';
				pkg[2] = ' ';
				pkg[3] = 'U';
				pkg[4] = '\0';
			}

			else{
				pkg[0] = 'S';
				pkg[1] = 'V';
				pkg[2] = ' ';
				pkg[3] = 'D';
				pkg[4] = '\0';
			}
			sendPKG(pkg, strlen(pkg));

			break;
		case 0xe: // ping
			
			pkg[0] = 'P';
			pkg[1] = '\0';
			
			sendPKG(pkg, strlen(pkg));
			//printf("\n***%s***\n", pkg);
			
			break;
		case 0xf: // LED
			
			pkg[0] = 'F';
			pkg[1] = '\0';
			
			sendPKG(pkg, strlen(pkg));
			//printf("\n***%s***\n", pkg);
			
			break;
			
		default:
			return 0;
		}
	return 0;
}

int sendPKG (unsigned char *buf, int len){
	
	printf("%s\n", buf); // Print out the buffer (what is going to be sent to the Nucelo)
}

