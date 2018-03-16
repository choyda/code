/*************************************************************************
 > File Name: setsid.c
 > Author: 
 > Mail: 
 > Created Time: 二 12/26 16:16:52 2017
 ************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){

    pid_t pid;

    if(pid = fork()){
        printf("i am prent pid %d\n", getpid());
		sleep(1);
        return 0;
    }

    printf("i am pid %d\n", getpid());
    printf("i am gid %d\n", getpgrp());
    printf("i am sid %d\n", getsid(0));

    setsid(); //子进程设置完会话id，下面的id都是他自己

    printf("i am pid %d\n", getpid());
    printf("i am gid %d\n", getpgrp());
    printf("i am sid %d\n", getsid(0));
    return 0;
}
