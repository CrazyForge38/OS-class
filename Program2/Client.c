#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h> 


int main(int argc, char* argv[])
{

int pid = getpid();
char strPid[4];

sprintf(strPid, "%d", pid);


if (mkfifo(strPid, 0777) == -1) { //error checking
    if (errno != EEXIST) { //checks if it exists
	printf("Could not create fifo file\n"); 
      return 1;
    }
  }
//start initializing
int sysCall, inputval, results, num_param;
//char fifoName[] = "FIFO2";
//open write mode



printf("Process ID: %d\n", pid);



//send fifoname to start client specific fifo
printf("enter system call 1 to connect to server ");
scanf("%d", &sysCall);

int writer = open("Server", O_WRONLY);
if (writer == -1) { return 2; }




printf("Sending process id %d to server\n", pid);




int cont;




while(1){
	
	open("Server", O_WRONLY);
	

	
	
	
	write(writer, &pid, sizeof(int));
	if(sysCall == 1){
	printf("system call number requested: %d\n", sysCall);
	write(writer, &sysCall, sizeof(int));
	}
	else{
	
	//choice menu
	int choice;
	printf("Choose one of the following:\n 1- Send request to server\n 2-Exit\n 3-Terminate\n");
	scanf("%d", &choice);
	if (choice == 1){
		printf("Which system call? 2- add\n 3- subtract\n 4- multiply\n 5- divide\n 6-store\n 7- recall\n");
		scanf("%d",&sysCall);
		printf("system call number requested: %d\n", sysCall);
		write(writer, &sysCall, sizeof(int));
	}
	if (choice == 2){
		sysCall = 0;
		printf("System call 0 sent, exiting now\n");
		write(writer, &sysCall, sizeof(int));
		
		}
	if (choice == 3){
		sysCall= -1;
		printf("system call -1 sent, terminating now\n");
		write(writer, &sysCall, sizeof(int));	
	}
}//end else
	if (sysCall == 6){
		num_param = 1;
		write(writer, &num_param, sizeof(int));
		printf("Enter a number: ");
		scanf("%d", &inputval);
		write(writer, &inputval, sizeof(int));
		
	}
	if (sysCall == 2 || sysCall == 3 || sysCall == 4 || sysCall == 5) {
		num_param = 2;
		write(writer, &num_param, sizeof(int));
		
		printf("Enter first number: ");
		scanf("%d", &inputval);
		write(writer, &inputval, sizeof(int));
		
		printf("Enter second number: ");
		scanf("%d", &inputval);
		write(writer, &inputval, sizeof(int));
		

	}
	if(sysCall == 7 || sysCall == 1){
		num_param = 0;
		write(writer, &num_param, sizeof(int));
		}

	if(sysCall == 0 || sysCall ==-1){
		num_param = 0;
		write(writer, &num_param, sizeof(int));
		printf("closing client\n");
	}
	

	
	close(writer);
	//open read mode
	int reader = open(strPid, O_RDONLY);
	if (reader == -1) { return 2; }
	
	if(sysCall == 7){
		//num_param = 0;
		read(reader, &results, sizeof(int));
		printf("Recalled: %d\n", results);
	}
	
	if (sysCall == 2 || sysCall == 3 || sysCall == 4 || sysCall == 5) {
	read(reader, &results, sizeof(int));
		printf("returned result: %d\n",results);
	}
	if(sysCall == 6){
	read(reader, &results, sizeof(int));
		printf("Stored: %d\n", results);
	}
	if(sysCall == 0 || sysCall == -1){break;}
	if(sysCall == 1){
	read(reader, &pid, sizeof(int));
	printf("New ID: %d\n", pid);
	}
		sysCall = 0;
		close(reader);
	printf("continue client %d? ", pid);
	scanf("%d", &cont);
}//end while

}//end main
