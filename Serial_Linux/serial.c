// Reference and Credit:  https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/

// Find out more on this great tutorial site!



// C library headers

#include <stdio.h>

#include <string.h>

#include <stdlib.h>



// Linux headers

#include <fcntl.h> // Contains file controls like O_RDWR

#include <errno.h> // Error integer and strerror() function

#include <termios.h> // Contains POSIX terminal control definitions

#include <unistd.h> // write(), read(), close()



int main(int argc, char* argv[]) {

 

 /*

 if (argc  <= 1)

 {

 printf("Please supply which serial port you would like to use!");

 exit(0);

 }

 

 if( strcmp(argv[1], "/dev/ttyS1") || strcmp(argv[1], "/dev/ttyS2") || strcmp(argv[1], "/dev/ttyS3") || strcmp(argv[1], "/dev/ttyS4"))

 {

 

 }

 else

 {

 printf("The serial port you have entered is invaild");

 exit(1);

 }

 

 printf("Serial Port: %s \n", argv[1]);

 */

 

 

  int serial_port = open("/dev/ttyS1", O_RDWR);  



// Configuring the serial port!







  // Create new termios struc, we call it 'tty' for convention

  struct termios tty; //form to be submitted to the Linux OS for configuring the serial port



  // Read in existing settings, and handle any error

  if(tcgetattr("/dev/ttyS1", &tty) != 0) {

      printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));

      return 1;

  }

  



  // Convention here:

  // "&= ~" is ANDing a negated bit, so it will clear the bit

  // "|=" is ORing a positive bit, so it will set the bit

  

  

  // ***Local Mode***

  

  // IMPORTANT - parity bit setting, size setting, and stop bit setting.  Match with Embedded target!

  tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)

  tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)

  tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size 

  tty.c_cflag |= CS8; // 8 bits per byte (most common)

  

  

  //no so important for DSUB9

  //[RESEARCH]: The following flags are by default always set up this way, unless you are dealing with dial-up MODEM. Why?

  tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)

  tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

  

  //Maybe important?

  tty.c_lflag &= ~ICANON;  // [RESEARCH]: what is canonical mode mean?  When to set and when to clear?

  

  //Not so important.  more about formatting and confirmation of transmission.

  //[RESEARCH]: any impact if these echo-related flags are set or cleared in non-canonical mode?

  tty.c_lflag &= ~ECHO; // Disable echo

  tty.c_lflag &= ~ECHOE; // Disable erasure

  tty.c_lflag &= ~ECHONL; // Disable new-line echo

  

  //keep in default.

  //Often kept cleared as default.  

  tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP

  

  

  // ***Input Mode***

  

  //Keep these settings in default to make sure we are working with raw data

  //[RESEARCH]: What do these flags do?

  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl

  tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes





  // ***Output Mode***

  

  //Keep these settings in default to make sure we are not sending out data with special character treatment.

  tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)

  tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed 

  // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)

  // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)





  // ***Block Time Settings**

  tty.c_cc[VTIME] = 10;    //VTIME determines the block time for read()

  tty.c_cc[VMIN] = 0;      //VMIN determines the min number of characters rcv'd before unblocking read()

  

  

	// VMIN = 0, VTIME = 0: 	read() is No blocking, return immediately with what is available in the read buffer received from the com port.

	

	

	

	// VMIN > 0, VTIME = 0: 	This will make read() always wait for bytes (timed block on read()) 

	//				exactly how many is determined by VMIN), so read() could block indefinitely.

	// In this mode, read "ALWAYS" wait for contents from the serial port

	// But it would be determined by the number of characters received defined by VMIN

	

	

	

	// VMIN = 0, VTIME > 0: 	This is a blocking read() of any number chars with a maximum timeout (given by VTIME). 

	//				read() will block until either any amount of data is available, or the timeout occurs.

	// read() will block for VTIME amount of time, and unblock if we have rcv'd any data

	 

	 

	

	// VMIN > 0, VTIME > 0: 	Block until either VMIN characters have been received, or VTIME after first character has elapsed. 

	//							Note that the timeout for VTIME does not begin until the first character is received.

	

	

	//the number shoudl be in milliseconds (look up to confirm!)

	

	



  // Set in/out baud rate to be 9600

  cfsetispeed(&tty, B9600);

  cfsetospeed(&tty, B9600);

  

  



  // Save tty settings, also checking for error

  if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {

      printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));

      return 1;

  }

  

  

  

  

  

  



  // Write to serial port

  unsigned char msg[] = { 'H', 'e', 'l', 'l', 'o', '\r' };

  write(serial_port, "Hello, world!", sizeof(msg));



  // Allocate memory for read buffer, set size according to your needs

  char read_buf [256];



  // Normally you wouldn't do this memset() call, but since we will just receive

  // ASCII data for this example, we'll set everything to 0 so we can

  // call printf() easily.

  memset(&read_buf, '\0', sizeof(read_buf));



  // Read bytes. The behaviour of read() (e.g. does it block?,

  // how long does it block for?) depends on the configuration

  // settings above, specifically VMIN and VTIME

  int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));



  // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.

  if (num_bytes < 0) {

      printf("Error reading: %s", strerror(errno));

      return 1;

  }



  // Here we assume we received ASCII data, but you might be sending raw bytes (in that case, don't try and

  // print it to the screen like this!)

  printf("Read %i bytes. Received message: %s", num_bytes, read_buf);



  close(serial_port);

  return 0; // success

}
