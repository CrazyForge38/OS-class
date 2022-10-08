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
char strPid[5];
int pid, scn, val1, val2, results, param_c, cont;  
pid = getpid(); 

sprintf(strPid, "%d", pid);


if (mkfifo(strPid, 0777) == -1) { //error checking
    if (errno != EEXIST) { //checks if it exists
	printf("Could not create fifo file\n"); 
      return 1;
    }
  }
  int writer = open("Server", O_WRONLY);
  if (writer == -1) {printf("0"); return 2; }
  
  
if (mkfifo("Server", 0777) == -1) { 
  if (errno != EEXIST) { 
    return 1;
  }
}

printf("enter 1 to initiate connection with server: ");
scanf("%d", &scn);

printf("Sending PID: %d to server\n", pid);

while(1){
	open("Server", O_WRONLY);
	if(scn == 1){
	  printf("[+] initiating connection with server...\n");
	  if (write(writer, &pid, sizeof(int)) == -1) { return 3; } 
	  printf("SCN: %d\n", scn);
	  if (write(writer, &scn, sizeof(int)) == -1) {return 3;}
	}
	else{
	  int choice;
	  printf("Select a state for pid %d:\n 1-Send request to server\n 2-Exit client\n 3-Terminate server connection\n => ", pid);
	  scanf("%d", &choice);
	  if (write(writer, &pid, sizeof(int)) == -1) {return 3;}
	if (choice == 1){
		printf("\n Select a SCN: \n  2- add\n  3- subtract\n  4- multiply\n  5- divide\n  6- store\n  7- recall\n  =>");
		scanf("%d",&scn);
		printf("\nSending SCN: %d\n", scn);
		if (write(writer, &scn, sizeof(int)) == -1) {return 3;}
	}
	if (choice == 2){
		scn = 0;
		printf("System call 0 sent, exiting now\n");
		if (write(writer, &scn, sizeof(int)) == -1) {return 3;}
		
		}
	if (choice == 3){
		scn= -1;
		printf("system call -1 sent, terminating now\n");
		if (write(writer, &scn, sizeof(int)) == -1) {return 3;}	
	}
}

	if (scn == 6){
		param_c = 1;
		if (write(writer, &param_c, sizeof(int)) == -1) {return 4;}
		printf("Val1: ");
		scanf("%d", &val1);
		if (write(writer, &val1, sizeof(int)) == -1) {return 4;}
		
	}
	if (scn == 2 || scn == 3 || scn == 4 || scn == 5) {
		param_c = 2;
		if (write(writer, &param_c, sizeof(int)) == -1) {return 4;}
		
		printf("Val1: ");
		scanf("%d", &val1);
		if (write(writer, &val1, sizeof(int)) == -1) {return 4;}
		
		printf("Val2: ");
		scanf("%d", &val2);
		if (write(writer, &val2, sizeof(int)) == -1) {return 4;}
		

	}
	if(scn == 7 || scn == 1){
		param_c = 0;
		if (write(writer, &param_c, sizeof(int)) == -1) {return 4;}
		}

	if(scn == 0 || scn ==-1){
		param_c = 0;
		if (write(writer, &param_c, sizeof(int)) == -1) {return 4;}
	}
	
	close(writer);
	int reader = open(strPid, O_RDONLY);
	if (reader == -1) { return 2; }
	
	if(scn == 7){
		if (read(reader, &results, sizeof(int)) == -1) {return 5;}
		printf("Recalled: %d\n", results);
	}
	
	if (scn == 2 || scn == 3 || scn == 4 || scn == 5) {
	  if (read(reader, &results, sizeof(int)) == -1) {return 5;}
		printf("returned result: %d\n",results);
	}
	if(scn == 6){
	  if (read(reader, &results, sizeof(int)) == -1) {return 5;}
		printf("Stored: %d\n", results);
	}
	if(scn == 0 || scn == -1){
	  if(scn == 0)
		  printf("[+] Closing Client\n");
		else {
		  printf("[+] Killing Server\n");
		}
	  return 0;
	  break;
	}
	
	if(scn == 1){
	if (read(reader, &pid, sizeof(int)) == -1) {return 6;}
	printf("New ID: %d\n", pid);
	}
		scn = 0;
		close(reader);
	printf("---------------------------------\n");
}//end while

}//end main
