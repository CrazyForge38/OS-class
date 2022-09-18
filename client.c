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
  
  int fd = open("mainFIFO", O_WRONLY);
  if (fd == -1) {printf("0"); return 2; }
  int usrinput1,usrinput2 = 0;
  int pid,scn,param_c,value1,value2,results;
  scn =1;
  param_c = 1;
  pid = getpid();
  
  if (write(fd, &pid, sizeof(int)) == -1) { return 3; }
  printf("pid: %d\n", pid);
  if (write(fd, &scn, sizeof(int)) == -1) { return 3; }
  printf("scn: %d\n", scn);
  if (write(fd, &param_c, sizeof(int)) == -1) { return 3; }
  printf("param_c: %d\n", param_c);
  printf("-----------------------\n");
  
  while (1) {
  if (write(fd, &pid, sizeof(int)) == -1) { return 3; } //write pid
  //Now we are asking the user to select what they want
  printf("pid: %d\n", pid);
  printf("enter a number -1 to 7: ");
  scanf("%d", &scn); 
  printf("scn input: %d\n", scn);
  //they all will send the pid and the a SCN
  if (write(fd, &scn, sizeof(int)) == -1) { return 3; } //write usrinput
  //sectioning them off by param_c
  
  if(scn == 1) {
    param_c = 1;
    if (write(fd, &param_c, sizeof(int)) == -1) { return 3; }
  }
  if (scn == 6) { // without 1 bc thats only called once
    param_c = 1; // 1 param
    if (write(fd, &param_c, sizeof(int)) == -1) { return 3; }
    printf("Enter a number: ");
    scanf("%d", &usrinput2);
    if (write(fd, &usrinput2, sizeof(int)) == -1) { return 3; } // sending 1 param 
  }
  if (scn == 0 || scn == -1 || scn ==7) {
  }
  
  if (scn == 2 || scn == 3 || scn == 4 || scn == 5) {
    param_c = 2;
    if (write(fd, &param_c, sizeof(int)) == -1) { return 3; }
    printf("Enter your first number: ");
    scanf("%d", &usrinput1);
    if (write(fd, &usrinput1, sizeof(int)) == -1) {  return 3;}
    printf("Enter your second number: ");
    scanf("%d", &usrinput2);
    if (write(fd, &usrinput2, sizeof(int)) == -1) { return 3;}
  }
  close(fd);
  open ("mainFIFO", O_RDONLY); 
  
  if (scn == 2 || scn == 3 || scn == 4 || scn == 5) {
    if(read(fd, &results, sizeof(int)) == -1) {return 3;  } 
    printf("Returned: %d\n", results);
  }
  if(scn == 6) {
    if(read(fd, &results, sizeof(int)) == -1) { return 3; } 
    printf("We saved: %d\n", results);
  }
  if(scn == 7) {
    if(read(fd, &results, sizeof(int)) == -1) { return 3; } 
    printf("We saved: %d\n", results);
  }
  if(scn == 0) {
    return 0;
  }
  if (scn == -1) {
    return 0;
  }
  if(scn == 1) {
    printf("connection made\n");
  }
  printf("------------------------------------\n");
  close(fd);
  open("mainFIFO", O_WRONLY);
}
  return 0;
}
