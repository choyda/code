/*************************************************************************
  > File Name: fork_shared.c
  > Author: 
  > Mail: 
  > Created Time: 四 11/30 17:33:53 2017
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char ** argv, char ** environ){

    int a = 100, i;
    pid_t pid;
    for(i=0;i<3;i++){
        pid = fork();
        if(pid == 0){
            //a = 200;
            printf("child, %d\n", a);
            break;
        }
        if(pid > 0){  //之所以子进程的a也随着增长，是因为重新fork了。重新继承a的值
            sleep(1);
            a +=100;
            printf("prent, %d\n", a);
        }
    }
}
