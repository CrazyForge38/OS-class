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
#include <semaphore.h> 
#include "pthread.h"
#include <sys/queue.h>

void semaphore_Scheduler(void);
int Calculate(void);
int semKey_Lookup(int); 
void insertIntoQueue_Sema(int);
int removeFromQueue_Sema();
void insertIntoQueue_Connected(int); 
int removeFromQueue_Connected();
void Remove_said_Client(int);

sem_t semaphore;
int semaphore_Value, fd;
static int memory = 0;
int pid,scn,param_c,val1,val2; 
static int simPid=0; //got rid of the non static memory
int array[33];
int Sema_Queue[33]; //requested to unlock
int Sema_Pid_connected[33]; //is connected
char strPid[6];

int queue_Sema_Connected[33];
int count_Connected = 0;
int queue_Sema_Wait[33];
int count_Wait = 0;



int main(int argc, char* argv[]) //lets st
{
  //printf("this is a test");
  //Remove_said_Client(1);
  printf("test semaphore\n");
  semaphore_Scheduler();
  printf("test semaphore\n");
  printf("test semaphore\n");

if (mkfifo("Server", 0777) == -1) { //if it does not already exist
    if (errno != EEXIST) { //checks if it exists
	printf("Could not create fifo file\n"); 
      return 1;
    }
  }
int reader = open ("Server", O_RDONLY); //reads from client

//int pid,scn,param_c,val1,val2;//can i get rid of this? 
//int memory, simPid=0;
//int array[33];
//int pid_Queue[33];
//char strPid[6];


while(1){ open("Server", O_RDONLY);

  //if (pid > 30) {
   // send the info just to connect
  //}

  //grabing pid scn and param_c
	if (read(reader, &pid, sizeof(int)) == -1) {return 2;}
	printf("[+] client PID: %d\n", pid);
	//printf("test");
	
	if (read(reader, &scn, sizeof(int)) == -1) {return 2;}
	printf("SCN: %d\n", scn);
	
	if (read(reader, &param_c, sizeof(int)) == -1) {return 2;} 
	printf("param_c: %d\n", param_c);
  //end of init data collection
	
	
	
	
	// decision based on param_c
	if(param_c == 0) {printf("param_c: N/A\n"); }

	if(param_c == 1){
		if (read(reader, &val1, sizeof(int)) == -1) {return 3;}
		printf("%d\n", val1);
	}
	
	if (param_c == 2){//theres not 1 value
		if (read(reader, &val1,sizeof(int)) == -1) {return 3;}
		printf("val1: %d\n", val1);
		if (read(reader, &val2,sizeof(int)) == -1) {return 3;}
		printf("val2: %d\n", val2);
	}
	/////////
	int sum, diff, mult, div =0;
	///////// for calculations
	
	close(reader);
	
  if (scn == 1){ // saves a new client into memory
		simPid+=1; //current id count 
		array[simPid] = pid;// creates simpid
		insertIntoQueue_Connected(pid); //throws into connected clients
	}//end of param_c
	
	if(pid<30){
	pid = array[pid];//relocated old pid
	}
	
	sprintf(strPid, "%d",pid);
	int writer = open(strPid, O_WRONLY);
	
	
	//calculator
	switch(scn){
		case 1:
			write(writer, &simPid, sizeof(int)); // we are sending the simulated pid
			break;
			
		case 2 : //add
			printf("val1: %d and val2: %d = ", val1, val2);
			sum = val1 + val2;
			if (write(writer, &sum, sizeof(int)) == -1) {return 4;}
			printf("%d\n",sum);
			break;
			
		case 3 : //sub
			printf("val1: %d and val2: = %d ", val1, val2);
			diff = val1 - val2;
			if (write(writer, &diff, sizeof(int)) == -1) {return 4;}
			printf("%d\n",diff);
			break;
		
		case 4 : //mul
			printf("val1: %d and val2: %d = ", val1, val2);
			mult = val1 * val2;
			if (write(writer, &mult, sizeof(int)) == -1) {return 4;}
			printf("%d\n",mult);
			break;
		
		case 5 : //div
			printf("val1: %d and val2: %d = ", val1, val2);
			div = val1 / val2;
			if (write(writer, &div, sizeof(int)) == -1) {return 4;}
			printf("%d\n",div);
			break;
			
		case 6 : // store
			memory = val1;
			printf("stored: %d\n", memory);
			if (write(writer, &memory, sizeof(int)) == -1) {return 4;}
			break;
		
		case 7 : // read from memeory
			printf("recalled: %d\n", memory);
			if (write(writer, &memory, sizeof(int)) == -1) {return 4;}
			break;
			
		case 0 : //exit 
			printf("[+] closing client FIFO\n");//if a client exits, remove him from the queue
			close(writer);	//remove and shift
			//if (scn == 1){ // saves a new client into memory
		    //simPid+=1; //current id count 
		    //array[simPid] = pid;// creates simpid
		    //removeFromQueue_Connected(pid); //removes connected client but not the one we want to remove unless we manipulate the queue to push what everone we are moving to the front!
	    //}
			break;
		case -1://terminate
			printf("[+] terminating\n");
			close(writer);	
			return 1;
			break;
		default:
			printf("[+] SCN is invlaid\n" );
			break;
		
	}
  printf("---------------------------\n");
	//if(scn == -1){break;}//??
	close(writer);	

}	
	//return 0;
}
 

void semaphore_Scheduler(void) {
  char to_Lock[25] = "The key has been locked";
  char not_To_Lock[30] = "The key has been unlocked";
  char Failed[25] = "failure!";
	int sys_Call = 1;
	int sema_Val;
	static int sema_Queue_Index = 0;
	sem_init(&semaphore, 1, 1);
  //Need sim id
  
  
  if (sys_Call == 1) { //To Lock
    insertIntoQueue_Sema(pid); //this throws the pid into the queue
		sem_wait(&semaphore); //To lock and waits if semaphore is == 0
		printf("%s\n", to_Lock);
		printf("this sema == %d\n", sem_getvalue(&semaphore, &sema_Val));
  }
  if (sys_Call == 2) { //Or not to Lock
    if (sem_getvalue(&semaphore, &sema_Val) == 0 && semKey_Lookup(0) != pid){ //Not locked by this client
      //print failure and send
    }
    if (sem_getvalue(&semaphore, &sema_Val) == 0 && semKey_Lookup(0) == pid){ //locked by this client and no other in queue
      //post and acknowledge
        //if(others in queue)
          //give him the key and tell him
    }
  }
  if(sys_Call == 3) {
    printf("Queue is : ");
    for(int i = 0; i <= count_Connected; i++){
      printf("%d ", queue_Sema_Connected[i]);
    }
    //write to client the queue of clietns
  }
}

int semKey_Lookup(int pid_Q) {
  //who has the key and we could also do a send all of the pids
  int keyholder;
  keyholder = Sema_Queue[0]; //who ever is the first person
  return keyholder;
}

void Remove_said_Client(int num){
  printf("this is a test for rearrange");
  int arr_Store[333];
  int i = 0;
  while (1 < 2){
    int val = removeFromQueue_Connected();
    if (val == num) {
      for (int k = 0; k < i; k++){
        insertIntoQueue_Connected(arr_Store[k]);
        //return; //get out of this method
        //break;//does this get me out of the foor or while loop?
      }
      return;
    }
    arr_Store[i] = val;
    i++;
  }
}

//////////////////////////////////////////////////////////////////
void insertIntoQueue_Connected(int x) {//nothing to do with the key, they are just connected
    if (count_Connected == 33) { //what about removing what ever is conencted? i.e scn == 0 of some random client
        fprintf(stderr, "No more space in the queue\n");
        return;
    }
    queue_Sema_Connected[count_Connected] = x;
    count_Connected++;
}

int removeFromQueue_Connected() {
    if (count_Connected == 0) {
        fprintf(stderr, "No elements to extract from queue\n");
        return -1;
    }
    int res = queue_Sema_Connected[0];
    int i;
    for (i = 0; i < count_Connected - 1; i++) {
        queue_Sema_Connected[i] = queue_Sema_Connected[i + 1];
    }
    count_Connected--;
    return res;
}
//////////////////////////////////////////////////////////////////////
void insertIntoQueue_Sema(int x) {//for the ones wanting to grab the key
    if (count_Wait == 33) {
        fprintf(stderr, "No more space in the queue\n");
        return;
    }
    queue_Sema_Wait[count_Wait] = x;
    count_Wait++;
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
    return res;
}
///////////////////////////////////////////////////////////////////////
