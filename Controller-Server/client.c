#include "server-ip.h"

/* Global Variables */
unsigned char buffer[2];

int status = 0;
int client_socket, len;
struct sockaddr_in server_addr;
struct hostent *host;

/*
 * Recieve read data from the controller
 */
int sendP(void){
	
	buffer[0] = 'P';
	buffer[1] = '\0';


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
	server_addr.sin_addr.s_addr = inet_addr("192.168.0.20"); // Change address to local Computer
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
	printf ("msg sent\n");
		
	/* Close the client socket */
	close (client_socket);
	
	//sleep(1);

		
	return 0;
}


