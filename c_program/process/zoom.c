/*************************************************************************
 > File Name: wait.c
 > Author: 
 > Mail: 
 > Created Time: 二 12/ 5 10:06:01 2017
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
//僵尸进程，子进程先死亡，但是父进程还在运行，此时子进程就是僵尸进程
int main(int argc, char ** argv, char ** env){
    pid_t pid;
    pid = fork();
    if(pid ==0){
        //子进程运行10秒后死亡
        sleep(10);
        printf("子进程%d\n",getpid());
    }
    if(pid > 0){
        while(1){
            //父进程一直循环执行
            sleep(1);
            printf("父进程\n");
        }
    } 

}
