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


int main(int argc, char* argv[]) //lets st
{

if (mkfifo("Server", 0777) == -1) { //if it does not already exist
    if (errno != EEXIST) { //checks if it exists
	printf("Could not create fifo file\n"); 
      return 1;
    }
  }
int reader = open ("Server", O_RDONLY); //reads from client

int pid,scn,param_c,val1,val2; 
int memory, simPid=0;
int array[33];
char strPid[6];



while(1){ open("Server", O_RDONLY);
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
	if (scn == 1){ // what does this do
		simPid+=1;
		array[simPid] = pid;
	}//end of param_c
	
	
	
	
	
	close(reader);
	if(pid<30){
	pid = array[pid];//and this
	}
	
	sprintf(strPid, "%d",pid);
	int writer = open(strPid, O_WRONLY);
	
	
	//calculator
	switch(scn){
		case 1:
			write(writer, &simPid, sizeof(int));
			break;
			
		case 2 : //add
			printf("returning: %d + %d = ", val1, val2);
			sum = val1 + val2;
			if (write(writer, &sum, sizeof(int)) == -1) {return 4;}
			printf("%d\n",sum);
			break;
			
		case 3 : //sub
			printf("returning: %d - %d = ", val1, val2);
			diff = val1 - val2;
			if (write(writer, &diff, sizeof(int)) == -1) {return 4;}
			printf("%d\n",diff);
			break;
		
		case 4 : //mul
			printf("returning: %d * %d = ", val1, val2);
			mult = val1 * val2;
			if (write(writer, &mult, sizeof(int)) == -1) {return 4;}
			printf("%d\n",mult);
			break;
		
		case 5 : //div
			printf("returning: %d / %d = ", val1, val2);
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
			printf("[+] closing client FIFO\n");
			close(writer);	
			break;
		case -1://terminate
			printf("[+] terminating\n");
			close(writer);	
			break;
		default:
			printf("[+] SCN is invlaid\n" );
			break;
		
	}
  printf("---------------------------\n");
	if(scn == -1){break;}//??
	close(writer);	

}	
	//return 0;
}
 
