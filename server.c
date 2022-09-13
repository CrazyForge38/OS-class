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


int main(int argc, char* argv[])
{
  int memory = 0;

  if (mkfifo("mainFIFO", 0777) == -1) { //returns 1 if the fifo files returns an error when trying to make it
    if (errno != EEXIST) { //if it does not already exist
      printf("Could not create fifo file\n"); 
      return 1;
    }
  }
  int fd = open ("mainFIFO", O_RDONLY); //creates a fifo serverfifo in read only but not calling it
  if (fd == -1) {return 2;} //this lets me know if the fifo was not created properly

  int pid,scn,param_c,value1,value2;


  while (1) {
  open ("mainFIFO", O_RDONLY);
  if(read(fd, &pid, sizeof(int)) == -1) { return 3; } //grabs value from the client
  printf("read pid: %d\n", pid);
  if(read(fd, &scn, sizeof(int)) == -1) { return 3; } //since the program does not contiune 
  printf("read scn: %d\n", scn);
  if(read(fd, &param_c, sizeof(int)) == -1) { return 3; } //until the fifo has been open 
  printf("read param_c: %d\n", param_c);
  
  if (param_c == 0) { printf("no content\n");}//values do nothing                           //from both sides, this method 
    if(param_c == 1) {                                    // should work
      if(read(fd, &value1, sizeof(int)) == -1) { return 3; }
      printf("read value1: %d\n", value1);
    } else {
        if(read(fd, &value1, sizeof(int)) == -1) { printf("read val1\n"); return 3; }
        printf("read value1: %d\n", value1);        
        if(read(fd, &value2, sizeof(int)) == -1) { printf("read val2\n"); return 3; }
        printf("read value2: %d\n", value2);
      }
    printf("testing\n");
    close(fd);
       
    printf("Client pid: %d\n", pid);
    printf("System Call Requested: %d with %d parameter\n", scn, param_c);
    int sum,diff,mulp,div = 0;
    open ("mainFIFO", O_WRONLY); 
       
    switch(scn) {
      case 1 :
         break;
      case 2 :
         printf("param1: %d and param2: %d\n", value1, value2);
         sum = value1 + value2;
         if(write(fd, &sum, sizeof(int)) == -1) { return 4; }
         printf("Wrote %d\n", sum);
         break;
      case 3 :
         printf("param1: %d and param2: %d\n", value1, value2);
         diff = value1 - value2;
         if(write(fd, &diff, sizeof(int)) == -1) { return 4; }
         printf("Wrote %d\n", diff);
         break;
      case 4 :
         printf("param1: %d and param2: %d\n", value1, value2);
         diff = value1 * value2;
         if(write(fd, &mulp, sizeof(int)) == -1) { return 4; }
         printf("Wrote %d\n", mulp);
         break;
      case 5 :
         printf("param1: %d and param2: %d\n", value1, value2);
         diff = value1 / value2;
         if(write(fd, &div, sizeof(int)) == -1) { return 4; }
         printf("Wrote %d\n", div);
         break;
      case 6 :
         memory = value1;
         printf("stored: %d\n", memory);
         break;
      case 7 :
         if(write(fd, &div, sizeof(int)) == -1) { return 4; }
         printf("Recalled: %d", memory);
         break;
      case 0 :
         printf("Do nothing\n"); //do nothing
         break;
      case -1 :
         printf("terminating..\n");
         return 0;
         break;
      default :
         printf("Invalid grade\n" );
   } 
   close(fd);
       return 0;
  } 

  return 0;
} 
