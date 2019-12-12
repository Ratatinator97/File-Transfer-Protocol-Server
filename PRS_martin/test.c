#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>

int main(){
  pid_t ppid = getpid();
  printf("PPID:%d\n",ppid);
  int i =0;
  while(i<3 && getpid() == ppid){
    i++;
    printf("i:%d\n",i);
    pid_t pid = fork();
    if(pid == 0){
      printf("JE SUIS LE FILS - %d DU PERE  %d\n",getpid(),getppid());
    }else{
      printf("JE SUIS LE PERE - %d CREANT LE FILS %d\n",getpid(),pid);
    }
  }
  printf("JE SUIS %d et je quitte ce monde avec i=%d\n",getpid(),i);
  return(0);
}
