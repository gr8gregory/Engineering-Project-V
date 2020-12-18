/*
 * server-ip.c
 * This server is running on TCP protocol
 * This is a sample internet server application that will respond
 * to requests on port 5000
 */

/* Header File*/

#include "server-ip.h"



unsigned char buffer[4];


/*
 * this signal handler is used to catch the termination
 * of the child. Needed so that we can avoid wasting
 * system resources when "zombie" processes are created
 * upon exit of the child (as the parent could concievably
 * wait for the child to exit)
 */

void
SigCatcher (int n)
{
    wait3 (NULL, WNOHANG, NULL);    
	signal (SIGCHLD, SigCatcher);
}

int
main (int argc, char *argv[])
{
	int server_socket, client_socket;
	int client_len;
	struct sockaddr_in client_addr, server_addr;
	int len, i;
	int opt = 1;
	FILE *p;


	if (argc != 2) {
		printf ("usage: c PORT_NUMBER\n");
		return 1;
	}	/* endif */

	/*
	 * install a signal handler for SIGCHILD (sent when the child terminates)
	 */

	signal (SIGCHLD, SigCatcher);

	/*
	 * obtain a socket for the server
	 */

	if ((server_socket = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
		printf ("grrr, can't get the server socket\n");
		return 1;
	}	/* endif */

	/*
	 * initialize our server address info for binding purposes
	 */
	if ((setsockopt(server_socket,SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt , sizeof(opt))) ) {
		printf ("Failed to set socket options!\n");
		return 1;
	}	/* endif */

	memset (&server_addr, 0, sizeof (server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl (INADDR_ANY);
	server_addr.sin_port = htons (5000);

	

	if (bind (server_socket, (struct sockaddr *)&server_addr, sizeof (server_addr)) < 0) {
		printf ("grrr, can't bind server socket\n");
		close (server_socket);
		return 2;
	}	/* endif */

	/*
	 * start listening on the socket
	 */

	if (listen (server_socket, 5) < 0) {
		printf ("grrr, can't listen on socket\n");
		close (server_socket);
		return 3;
	}	/* endif */

	/*
	 * for this server, run an endless loop that will
	 * accept incoming requests from a remote client.
	 * the server will fork a child copy of itself to handle the
	 * request, and the parent will continue to listen for the
	 * next request
	 */
	
	printf("Listing....\n");
	while (1) {
		/*
		 * accept a packet from the client
		 */

		client_len = sizeof (client_addr);
		if ((client_socket = accept (server_socket, 
		(struct sockaddr *)&client_addr, &client_len)) < 0) {
			printf ("grrr, can't accept a packet from client\n");
			close (server_socket);
			return 4;
		}	/* endif */

		/*
		 * fork a child
		 */


		if (fork() == 0) {
			/*
			 * this is done by CHILD ONLY!
			 *
			 * read a block of info max BUFSIZE. 
			 */

			read (client_socket, buffer, BUFSIZ);

			/* print the incoming buffer */
			printf("Incoming Buffer: %x%x%x\n\n", buffer[0],buffer[1],buffer[2]);
			serverOutput(buffer);

			close (client_socket);
			return 0;
		}		
		else {
			/*
			 * this is done by parent ONLY
			 */

			close (client_socket);
		}	/* endif */
	}	/* end while */

	return 0;
}	/* end main */


