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

void connect_To_Server();
void wait_sem();
void send_Value(int);
int grab_Results();
void functionality();

char strPid[5];
int pid, scn, val1, val2, results, param_c, cont,scn_State2,value; 
int reader, writer;
int semaphore = 0;

int main(int argc, char* argv[])
{
	pid = getpid(); 

	sprintf(strPid, "%d", pid);
  printf("test");

	if (mkfifo(strPid, 0777) == -1) { //if it does not already exist
		  if (errno != EEXIST) { //checks if it exists
		printf("Could not create fifo file\n"); 
		    return 1;
		  }
		}
		
	if (mkfifo("Server", 0777) == -1) { 
		if (errno != EEXIST) { 
		  return 1;
		}
	}

///////////////////////////////////////////////////////////////////////
  connect_To_Server();//gives sim id
	while(1){
	  printf("this is now the main loop\nEnter 1 for wait() and 2 for post/ps()\n");
	  scanf("%d", &scn);
	  send_Value(scn);
	  if (scn == 1) { // wait
	    printf("[+] we are calling wait()\n");
	    wait_sem();
	  }
	  if (scn == 2) { //post & ps
	  
	  }
	  sleep(1);
	}//end while

}//end main

void wait_sem() {
  printf("[+] asking for the key\n");
  send_Value(scn);
  semaphore = grab_Results();//we should get 1 // i need to grab in server
  printf("[+] end of wait\n");
}

void connect_To_Server() {
  writer = open("Server", O_WRONLY);
  printf("first connection => send pid\n");
  if (write(writer, &pid, sizeof(int)) == -1) { exit(1); } 
  close(writer);
  reader = open(strPid, O_RDONLY);
  if (read(reader, &pid, sizeof(int)) == -1) {exit(1);}
  printf("new pid => %d\n", pid);//new pid!
  close(reader);
}

void functionality() {
  
}


void send_Value(int value) {
  writer = open("Server", O_WRONLY);
  if (write(writer, &value, sizeof(int)) == -1) { exit(1); } 
  close(writer);
  printf("[+]we wrote: %d\n => end of send\n", value);
}

int grab_Results() {
  reader = open(strPid, O_RDONLY);
  if (read(reader, &results, sizeof(int)) == -1) {exit(1);}
  close(reader);
  printf("[+]we got: %d\n", results);
  return results;
}
