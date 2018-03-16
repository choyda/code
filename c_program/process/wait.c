/*************************************************************************
 > File Name: wait.c
 > Author:
 > Mail:
 > Created Time: Wed 06 Dec 2017 11:20:24 AM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char ** argv, char ** env){

    pid_t pid, wpid;
    int i, status;

    pid = fork();

    if(pid == 0 ){
        execl("wait_err", "wait_err", NULL);
        //sleep(10);
        printf("I am child%d\n", getpid());
    }
    if(pid > 0){
        wpid = wait(&status);
        if(wpid == -1){
            perror("wait error:");
        }
        if(WIFEXITED(status)){ // 正常退出
            printf("子进程退出状态:%d\n", WEXITSTATUS(status));
        }
        if(WIFSIGNALED(status)){// 异常退出
            printf("子进程异常退出状态:%d\n", WTERMSIG(status));
        }
    }
    return 111;
}
