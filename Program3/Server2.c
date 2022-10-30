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

void grab_pid();
void wait_sem();
void post_sem();
void ps();

int pid,scn,param_c,val1,val2,counter,reader,writer,Key_Holder; 
int memory, simPid=0;
int array[33],wait_Queue[33];
char strPid[6];
char key_Given[3] = "key";
int semaphore = 1;

int main(int argc, char* argv[]) //lets st
{

	if (mkfifo("Server", 0777) == -1) { //if it does not already exist
		  if (errno != EEXIST) { //checks if it exists
		printf("Could not create fifo file\n"); 
		    return 1;
		  }
		}
	reader = open("Server", O_RDONLY); //reads from client
	writer = open(strPid, O_WRONLY);
	close(writer);
  
	
	while(1) {
	  printf("grabing the pid\n");
	  grab_pid();
	  printf("grabed pid\n");
	  if (read(reader, &scn, sizeof(int)) == -1) {return 33;}
	  printf("we grabed scn: %d\n", scn);
	  close(reader);
	  if(scn == 1) {
	    printf("grabing key\n");
	    wait_sem();
	  }
	  if(scn == 2) {
	  printf("we are posting");
	    post_sem();
	  }
	  if(scn == 3) {
	    ps();
	  }
	}
}
 
void ps() {
  printf("test oribntg\n");
}

void wait_sem() {
  if(pid<30){
	simPid = array[pid];//and this
	}
  writer = open(strPid, O_WRONLY);
  if(semaphore == 1) {
    semaphore--;
    scn = 44;
    printf("The key has been taken\n");
    //printf("the pid %d \n", pid);
    if (write(writer, &scn, sizeof(int)) == -1) {exit(20);}//writes to the 
    printf("They hold the key\n");
    Key_Holder = simPid;
  } else {
    wait_Queue[simPid]= simPid;///really need to look into the functionallity 
  }
  close(writer);
  printf("end of the wait\n\n");
}

void post_sem() {
  char failure[7] = "failure";
  char ack[3] = "ACK";
  writer = open(strPid, O_WRONLY);
  if(Key_Holder != simPid) {
    if (write(writer, &scn, sizeof(int)) == -1) {exit(30);}
  }
  if(Key_Holder == simPid) { //wait queue
    if (write(writer, &scn, sizeof(int)) == -1) {exit(31);}
    semaphore++;
  }
  if(Key_Holder == simPid) { //just null
    if (write(writer, &scn, sizeof(int)) == -1) {exit(32);}
    semaphore++;
    //give key to next person
  }
  close(writer);
}
 
void grab_pid () {
  reader = open("Server", O_RDONLY);
  if (read(reader, &pid, sizeof(int)) == -1) {exit(34);}//if pid > x then we know its a new call
  printf("we got the pid %d\n", pid);
  sprintf(strPid, "%d",pid);
  if (pid > 33){ // add to connected when a new clinet is here
    pid = simPid;
		simPid+=1;
		array[simPid] = pid;
		counter++;
	}//pid conversion  
}
