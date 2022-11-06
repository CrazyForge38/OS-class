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

void grab_Pid();
int grab_Results();
void send_Value(int);
void insertIntoQueue_Sema(int);
int removeFromQueue_Sema(); 

int pid,scn,param_c,val1,val2,reader,writer,results, oldPid; 
int memory, simPid, count_Wait =0;
int array_Connected[33];
char strPid[6];
int semaphore = 1;
int queue_Sema_Wait[33];

int main(int argc, char* argv[]) //lets st
{

	if (mkfifo("Server", 0777) == -1) { //if it does not already exist
		  if (errno != EEXIST) { //checks if it exists
		printf("Could not create fifo file\n"); 
		    return 1;
		  }
		}
	//int reader = open ("Server", O_RDONLY); //reads from client

	// new 
	
	while(1) {
	printf("we want to grab a pid\n");
	grab_Pid();
	scn = grab_Results();
	if(scn == 1) {
	  if(semaphore == 1) {//inifinite wait while no key
	    printf("[+] we are giving the key away to pid: %d\n", pid);
	    semaphore = 0;
	    send_Value(1); //gives key
	  //give key
	  }
	  //add to queue
	  insertIntoQueue_Sema(pid);
	}
	if(scn == 2 || scn == 3) {
	  if(scn == 3) {
	  
	  }
	  //post();
	}
	sleep(1);
	printf("[+]end of while loop\n");
	}
	
}//end of main

void grab_Pid() {
 printf("get pid here\n");
 reader = open("Server", O_RDONLY);
 if (read(reader, &pid, sizeof(int)) == -1) {exit(1);}
 close(reader);
 //printf("reader\n");
 if(pid > 33) {
   simPid+=1;
	 array_Connected[simPid] = pid;//stores old pid
   sprintf(strPid, "%d",pid); 
   pid = simPid;
   writer = open(strPid, O_WRONLY);//opens pid of the old name
   if (write(writer, &simPid, sizeof(int)) == -1) {exit(1);}
   close(writer);
   printf("this is the pid we grabed => %d\n", pid);
 }
}

//methods to grab and write from current pid 
int grab_Results() {
  reader = open("Server", O_RDONLY);
  if (read(reader, &results, sizeof(int)) == -1) {exit(1); }
  close(reader);
  printf("[+]we got: %d\n", results);
  return results;
}
void send_Value(int value) {
  oldPid = array_Connected[pid];//all new pid get sim pid
	sprintf(strPid, "%d",oldPid);
  writer = open(strPid, O_WRONLY);
  if (write(writer, &value, sizeof(int)) == -1) { exit(1); } 
  close(writer);
  printf("[+]we wrote: %d\n", value);
}
//////////////////////////////////////////////////////////
void insertIntoQueue_Sema(int x) {//for the ones wanting to grab the key
    if (count_Wait == 33) {
        fprintf(stderr, "No more space in the queue\n");
        return;
    }
    queue_Sema_Wait[count_Wait] = x;
    count_Wait++;
    printf("[+] We added %d to the queue\n", x);
}

int removeFromQueue_Sema() {
    if (count_Wait == 0) {
        fprintf(stderr, "No elements to extract from queue\n");
        return -1;
    }
    int res = queue_Sema_Wait[0];
    int i;
    for (i = 0; i < count_Wait - 1; i++) {
        queue_Sema_Wait[i] = queue_Sema_Wait[i + 1];
    }
    count_Wait--;
    printf("[+] We removed %d to the queue\n", res);
    return res;
}
