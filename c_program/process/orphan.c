/*************************************************************************
 > File Name: orphan.c
 > Author: 
 > Mail: 
 > Created Time: 五 12/ 1 11:01:31 2017
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
//孤儿进程，父进程结束，子进程还没结束，子进程结束由init进程进行回收。
int main(int argc, char ** agev, char ** env){
    int i;
    pid_t pid;
    pid = fork();

    if(pid == 0){
        printf("i am child,i pid %d\n", getpid());
        sleep(10);
    }
    if(pid > 0){
        printf("i am prent,i pid %d\n", getpid());
        sleep(3);
    }
}
