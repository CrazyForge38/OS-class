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

if (mkfifo("Server", 0777) == -1) { //error checking
    if (errno != EEXIST) { //checks if it exists
	printf("Could not create fifo file\n"); 
      return 1;
    }
  }

//open reader
int reader = open ("Server", O_RDONLY); //create serverfifo 

//initialize all values
int pid,sysCall,num_param,num1,num2;
int memory=0;
int counter = 0;
int array[30];
char strPid[4];


//infinite loop 
while(1){ open("Server", O_RDONLY);

	read(reader, &pid, sizeof(int));//get process ID from client
	printf("client Process ID: %d\n", pid);
	
	read(reader, &sysCall, sizeof(int));//let user choose an action
	printf("System call requested: %d\n", sysCall);
	
	read(reader, &num_param, sizeof(int)); // get number of parameters 				depending on system call number
	printf("with %d parameter/s which is/are: \n", num_param);

	
	
	//for no parameters
	if(num_param == 0) {printf("none given\n"); }

	//for two parameters
	if (num_param == 2){
		read(reader, &num1,sizeof(int));
		printf("parameter 1: %d\n", num1);
		read(reader, &num2,sizeof(int));
		printf("parameter 2: %d\n", num2);
	}
	
	//for one parameters
	if(sysCall == 6){
		read(reader, &num1, sizeof(int));
		printf("%d\n", num1);
	}
	
	if (sysCall == 1){
		counter+=1;
		array[counter] = pid;
	}
	
	int sum, diff, mult, div =0;
	close(reader);
	if(pid<30){
	pid = array[pid];
	}
	sprintf(strPid, "%d",pid);
	int writer = open(strPid, O_WRONLY);//write to client fifo
	switch(sysCall){
		case 1:
			write(writer, &counter, sizeof(int));
		
			break;//set up before the switch
			
		case 2 : //summation
			printf("%d + %d = ", num1, num2);
			sum = num1 + num2;
			write(writer, &sum, sizeof(int));
			printf("%d\n",sum);
			break;
			
		case 3 : //subtraction
			printf("%d - %d = ", num1, num2);
			diff = num1 - num2;
			write(writer, &diff, sizeof(int));
			printf("%d\n",diff);
			break;
		
		case 4 : //multiplication
			printf("%d * %d = ", num1, num2);
			mult = num1 * num2;
			write(writer, &mult, sizeof(int));
			printf("%d\n",mult);
			break;
		
		case 5 : //Divison
			printf("%d / %d = ", num1, num2);
			div = num1 / num2;
			write(writer, &div, sizeof(int));
			printf("%d\n",div);
			break;
			
		case 6 : // store to memory
			memory = num1;
			printf("stored: %d\n", memory);
			write(writer, &memory, sizeof(int));
			break;
		
		case 7 : // store to memory
			printf("recalled: %d\n", memory);
			write(writer, &memory, sizeof(int));
			break;
			
		case 0 : //exit 
			printf("closing client FIFO\n");
			close(writer);	
			break;
		case -1://terminate
			printf("terminating\n");
			close(writer);	
			break;
		default:
			printf("Invalid input\n" );
			break;
		
	}// end switch/case

	if(sysCall == -1){break;}
	close(writer);	
	

}//end while loop
	
	//return 0;
}//end main
 
