#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>//no need
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h> //allows you to open named pipes


int main(int argc, char* argv[]) //lets st
{
  if (mkfifo("mainFIFO", 0777) == -1) { //returns 1 if the fifo files returns an error when trying to make it
    if (errno != EEXIST) { //if it does not already exist
      printf("Could not create fifo file\n"); 
      return 1;
    }
  }
  int usrinput1,usrinput2;
  int pid,scn,param_c,value1,value2,results;
  pid = getpid();
  printf("%d\n", pid);
  printf("enter a number -1 to 7");
  scanf("%d", &usrinput1); // we got the system call number
  printf("input: %d\n", usrinput1);
  
  int fd = open("mainFIFO", O_WRONLY);
  if (fd == -1) { return 2; }
   
  if (write(fd, &pid, sizeof(int)) == -1) { return 3; }
  if (write(fd, &usrinput1, sizeof(int)) == -1) { return 3; }
  ////////////////////////////////////////////////////////////
  if (usrinput1 == 1 || usrinput1 == 6) {
    param_c = 0;
    if (write(fd, 0, sizeof(int)) == -1) { return 3; }
    printf("Does this get called\n");
  } // i can make this more readable for sure
  
  if (usrinput1 == 2 || usrinput1 == 3 || usrinput1 == 4 || usrinput1 == 5) {
    param_c = 2;
    if (write(fd, &param_c, sizeof(int)) == -1) { return 3; }
    printf("Enter your first number: ");
    scanf("%d", &usrinput2);
    if (write(fd, &usrinput2, sizeof(int)) == -1) { return 3; }
    printf("Enter your second number: ");
    scanf("%d", &usrinput2);
    if (write(fd, &usrinput2, sizeof(int)) == -1) { return 3; }
  }
  printf("what is here %d", usrinput1);
  if (usrinput1 == 7 || usrinput1 == 0 || usrinput1 ==-1) {
    param_c = 1;
    if (write(fd, &param_c, sizeof(int)) == -1) { return 3; }
    printf("Enter a number: ");
    scanf("%d", &usrinput2);
    if (write(fd, &usrinput2, sizeof(int)) == -1) { return 3; }
  } //ill need a return x if this doesnt input a number ////////////////////////////
  
  close(fd);
  open ("mainFIFO", O_RDONLY);
  
  if (usrinput1 == 1 || usrinput1 == 6) { 
    printf("Returned: nothing");
  } 
  
  if (usrinput1 == 2 || usrinput1 == 3 || usrinput1 == 4 || usrinput1 == 5) {
    if(read(fd, &results, sizeof(int)) == -1) { return 3; } 
    printf("Returned: %d\n", results);
  }
  printf("what is here %d", usrinput1);
  if (usrinput1 == 7 || usrinput1 == 0 || usrinput1 ==-1) {
    if(read(fd, &results, sizeof(int)) == -1) { return 3; } 
    printf("Returned: %d\n", results);
  } 
  
  close(fd);
   
  return 0;
}
