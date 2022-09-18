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
  static int memory = 0;

  if (mkfifo("mainFIFO", 0777) == -1) { //returns 1 if the fifo files returns an error when trying to make it
    if (errno != EEXIST) { //if it does not already exist
      printf("Could not create fifo file\n"); 
      return 1;
    }
  }
  int fd = open ("mainFIFO", O_RDONLY); //creates a fifo serverfifo in read only but not calling it
  if (fd == -1) {return 2;} //this lets me know if the fifo was not created properly

  int pid,scn,param_c,value1,value2;
  char str[30];


  while (1) {
    open ("mainFIFO", O_RDONLY);
    if(read(fd, &pid, sizeof(int)) == -1) { return 3; } //
    printf("read pid: %d\n", pid);
    if(read(fd, &scn, sizeof(int)) == -1) { return 3; } //
    printf("read scn: %d\n", scn);
    if(read(fd, &param_c, sizeof(int)) == -1) { return 3; } //
    printf("read param_c: %d\n", param_c);
/////////////////////////////////////////////////////////////////    
    if(param_c == 1) {//for 1 or 6
      if (scn == 6) { // for 6
       if(read(fd, &value1, sizeof(int)) == -1) { return 3; }
        printf("read value1: %d\n", value1); // grabed storing value
      } else { // for 1
        // do nothing
        }
      }
////////////////////////////////////////////////
    if (param_c == 0) {
      //Do nothing
    }
    //////////////////////////////////////
    if(param_c == 2 || param_c == 3 || param_c == 4 || param_c == 5) {
      if(read(fd, &value1, sizeof(int)) == -1) { printf("read val1\n"); return 3; }
      printf("read value1: %d\n", value1);        
      if(read(fd, &value2, sizeof(int)) == -1) { printf("read val2\n"); return 3; }
      printf("read value2: %d\n", value2);
    }
    //////////////////////////////////////
    close(fd);
    printf("Client pid: %d\n", pid);
    printf("System Call Requested: %d with %d parameter\n", scn, param_c);
    int sum,diff,mulp,div = 0;
    open ("mainFIFO", O_WRONLY); 
    switch(scn) {
      case 1 :
         printf("Client");
         printf("%d", pid);
         printf("FIFO\n");
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
         mulp = value1 * value2;
         if(write(fd, &mulp, sizeof(int)) == -1) { return 4; }
         printf("Wrote %d\n", mulp);
         break;
      case 5 :
          printf("param1: %d and param2: %d\n", value1, value2);
         div = value1 / value2;
         if(write(fd, &div, sizeof(int)) == -1) { return 4; }
         printf("Wrote %d\n", div);
         break;
      case 6 :
         memory = value1;
         printf("value1 = %d\n", value1);
         printf("storing: %d\n", memory);
         if(write(fd, &memory, sizeof(int)) == -1) { return 4; }
         printf("saved to memory\n");
         break;
      case 7 :
         if(write(fd, &memory, sizeof(int)) == -1) { return 4; }
         printf("Recalled: %d", memory);
         break;
      case 0 :
         printf("closing the client\n"); //close the client specific fifo and open the next
         break;
      case -1 :
         printf("CLosing the server and client");
         return 0; //closing the server
         break;
      default :
         printf("Invalid scn\n" );
   } 
   printf("--------------------------");
   printf("\n");
   close(fd);
  }

  return 0;
}
