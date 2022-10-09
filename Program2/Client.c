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
	int pid, scn, val1, val2, results, param_c, cont,scn_State2;  
	pid = getpid(); 

	sprintf(strPid, "%d", pid);


	if (mkfifo(strPid, 0777) == -1) { //if it does not already exist
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
///////////////////////////////////////////////////////////////////////
	while(1){
		open("Server", O_WRONLY); //contacts the server
		if(scn == 1){
			printf("[+] initiating connection with server...\n");
			if (write(writer, &pid, sizeof(int)) == -1) { return 3; } 
			printf("SCN: %d\n", scn);
			if (write(writer, &scn, sizeof(int)) == -1) {return 3;}
		}
		else{ //select the state the client will move to
			printf("Select a state for pid %d:\n 1-Send request to server\n 2-Exit client\n 3-Terminate server connection\n => ", pid);
			scanf("%d", &scn);
			if (write(writer, &pid, sizeof(int)) == -1) {return 3;}
		//if i have a if else if statement this should work without the new var. I will try that when i get a chance	
		
		if (scn == 1){
			printf("\n Select a SCN: \n  2- add\n  3- sub\n  4- mul\n  5- div\n  6- store\n  7- recall\n  =>");
			scanf("%d",&scn);
			printf("\nSending SCN: %d\n", scn);
			if (write(writer, &scn, sizeof(int)) == -1) {return 3;}
		}  else {
		  if (scn == 2){
		    //printf("does this: %d", scn);
		  	scn = 0;
		    printf("exiting client\n");
		  	if (write(writer, &scn, sizeof(int)) == -1) {return 3;}
			} else {
			  if (scn == 3){
		      //printf("does this: %d", scn);
		      scn= -1;
		      printf("terminating connection with Server\n");
		      if (write(writer, &scn, sizeof(int)) == -1) {return 3;}	
        }
			}
		}
	}// else if wasnt working but this did..


///////////////////// state 2 end and now write to the server the param_c and values

		
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
		
		if (scn == 6){
			param_c = 1;
			if (write(writer, &param_c, sizeof(int)) == -1) {return 4;}
			printf("Val1: ");
			scanf("%d", &val1);
			if (write(writer, &val1, sizeof(int)) == -1) {return 4;}
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
		printf("is this a scn: %d", scn);
		switch(scn){
		case 1:
		  if (read(reader, &pid, sizeof(int)) == -1) {return 6;}
		  printf("New ID: %d\n", pid);
		  scn = 0;
		  break;
		case 2 :
		case 3 :
		case 4 :
		case 5 :
			if (read(reader, &results, sizeof(int)) == -1) {return 5;}
			printf("returned result: %d\n",results);
			break;
	  case 7:
	    if (read(reader, &results, sizeof(int)) == -1) {return 5;}
			printf("Recalled: %d\n", results);
			break;
	  case 6: 
	    if (read(reader, &results, sizeof(int)) == -1) {return 5;}
			printf("Stored: %d\n", results);
			break;
	  case 0: 
	    printf("[+] Closing Client\n");
	    return 0;
	    break;
	  case -1:
				printf("[+] Killing Server\n");
			return 0;
			break;
			
		default:
			printf("[+] SCN is invlaid\n" );
			break;
	}/////////////////////////////////////////////////////////////////////////////////////////////////
			close(reader);
		printf("---------------------------------\n");
	}//end while

	}//end main
