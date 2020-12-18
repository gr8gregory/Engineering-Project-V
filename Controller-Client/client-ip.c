/*
 * client.c
 * This is running on TCP protocol
 *
 * This is a sample internet client application that will talk
 * to the server s.c via port of choice
 */

/*
 * This code is hardcoded to work on A3154-22 -> 10.121.112.149 5000
 */
 
/* Header Files */ 
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "client.h"
#include "controllerH.h"

/* Global Variables */
unsigned char buffer[4];

int status = 0;
int client_socket, len;
struct sockaddr_in server_addr;
struct hostent *host;

/*
 * Recieve read data from the controller
 */
int sendCMD(unsigned char cmd, unsigned short val){	
	
	/* Place the button hex value into index 0 */
	buffer[0] = cmd;
	
	/* Take unsigned short value read from the controller and split it into two index's of the buffer */
	buffer[1] = (val >> 8) & 0xff;
	buffer[2] = val & 0xff;
	
	/* Place a null terminator in the last element */
	buffer[3] ='\0';
	
	/* Sanity Check */
	//printf("Printing buffer: %x%x%x \n\n", buffer[0],buffer[1],buffer[2]);


	/*
	 * get a socket for communications
	 */
	if ((client_socket = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
		printf ("grrr, can't get a client socket!\n");
		return 3;
	}	/* endif */

	/*
	 * initialize struct to get a socket to host
	 */
	
	memset (&server_addr, 0, sizeof (server_addr));
	server_addr.sin_addr.s_addr = inet_addr("10.121.112.149"); // Hard coded IP address of lab computer here
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons (5000); // hard coded the lab computer port here

	/*
	 * attempt a connection to server
	 */

	if (connect (client_socket, (struct sockaddr *)&server_addr,
			sizeof (server_addr)) < 0) {
				printf ("grrr, can't connet to server!\n");
				close (client_socket);
				return 4;
	}	
	
	/*
	 * now that we have a connection, get a commandline from
	 * the user, and fire it off to the server
	 */
	
	/* Write the buffer to the Server */
	write (client_socket, buffer, strlen (buffer));

	/* Indicate the message was sent */
	//printf ("msg sent\n");
	
	/* Close the client socket */
	close (client_socket);
		
	return 0;
}



