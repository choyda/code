ii/*************************************************************************
 > File Name: waitpid.c
 > Author:
 > Mail:
 > Created Time: Wed 06 Dec 2017 02:44:41 PM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char ** argv, char ** env){

    pid_t pid, wpid;
    int i, status;
    pid = fork();
    if(pid == -1){
        perror("fork error:");
    }
    if(pid == 0){
        sleep(5);
        printf("i am child %d\n", getpid());
    }

    if(pid > 0){
        while(waitpid(-1,NULL,WNOHANG) == 0){
            sleep(1);
            printf("等待回收\n");
            printf("%d", wpid);
        }
        printf("回收成功%d,");

    }
    return 222;
}
