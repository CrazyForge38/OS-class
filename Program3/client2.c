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

void post_sem(int);
void wait_sem(int);
void ps();

int pid, scn, val1, val2, results, param_c, cont,scn_State2,writer,reader;
char strPid[5];//names pipe
int semaphore = 0; 
char key_Response[40];

int main(int argc, char* argv[])
{
	int pid, scn, val1, val2, results, param_c, cont,scn_State2;  
	pid = getpid(); //first pid
	sprintf(strPid, "%d", pid);

	if (mkfifo(strPid, 0777) == -1) { //if it does not already exist
		  if (errno != EEXIST) { //checks if it exists
		printf("Could not create fifo file\n"); 
		    return 1;
		  }
		}
  ////
	if (mkfifo("Server", 0777) == -1) { 
		if (errno != EEXIST) { 
		  return 1;
		}
	}
	writer = open("Server", O_WRONLY);
	if (writer == -1) {printf("0"); return 2; }
	
	
	//if (write(writer, &pid, sizeof(int)) == -1) {return 1;}//first write to send pid
	close(writer);
	
	while(1) {
	
	  if(semaphore == 1) {
	  	printf("Moving to critical\n");
			for (int x = 0; x < 6; x++) {
				printf("CRIT ");
				sleep(1);
				if (x == 5 ) {
					printf("\n");
				}
			}
			//we can ask the user to select 1 of them
			
			//ps();//print the stuff
			post_sem(semaphore);//give key back
		}
	  if (semaphore == 0) { //bc it cant do the other until the key is 1
	  //static that only grabs the new pid once or many times as long as it doesnt change
	    writer = open("Server", O_WRONLY);
	    if (write(writer, &pid, sizeof(int)) == -1) {return 1;}//first write to send pid
	    close(writer);
			for (int x = 0; x < 6; x++) {
			  printf("NON-CRIT ");
			  sleep(1);
			  if (x == 5 ) {
			    printf("\n");
			  }
			}
	    printf("waiting to get into the critical section\n");
	    wait_sem(semaphore);//waits for the key
	  }
  }
}

void ps() {
  printf("getting array\n");
}

void post_sem(int sema) {
  open("Server", O_WRONLY);
  printf("Enter 2: return the key & 3: Request connected clients\n");
	scanf("%d", &scn);
  if (write(writer, &scn, sizeof(int)) == -1) {exit(3);}
  printf("we sent scn: %d\n", scn);
  close(writer);
  reader = open(strPid, O_RDONLY);
  if (read(reader, &results, sizeof(int)) == -1) {exit(3);}//physcialy waits for the key
  printf("we got back: %d\n", results); //1 for sucees and 0 for failure && 2 for printing the string out
  if(results == 1) {//giving the key back
    semaphore = 0;
    printf("gave key back\n");
  } 
  if (results == 0) {
    printf("failure"); 
  }
  if( results == 3) {
    printf("Currently connected to the clients\n");//and give back key
    semaphore = 0;
    printf("gave key back\n");
  }
  close(reader);
}

void wait_sem(int sema) {
  open("Server", O_WRONLY);
  scn = 1;
  printf("asking for the key\n");
  if (write(writer, &scn, sizeof(int)) == -1) {exit(4);}
  close(writer);
  reader = open(strPid, O_RDONLY);
  printf("now we need the results\n");
  if (read(reader, &results, sizeof(int)) == -1) {exit(4);}//physcialy waits for the key
  printf("We can continue\n");
  semaphore = 1;
  close(reader);
  printf("end of wait\n");
}
