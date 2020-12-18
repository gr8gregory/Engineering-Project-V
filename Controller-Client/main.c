#include "controllerH.h"
#include "client.h"
#include <stdlib.h>

/* Just run the controller() Function */
int main (int argc, char *argv[]){
	
	
	/*int pid;
	pid = fork();
	if(pid < 0){
		
		printf("Forking Failed\n");
		exit(-1);
	}
	if(pid == 0){
		
		execl("server-ip","server-ip", NULL);
		exit(0);
	}
	else{*/
	
		controller();
		
	//}
	
	
	
}
