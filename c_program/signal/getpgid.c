/*************************************************************************
 > File Name: getpgid.c
 > Author: 
 > Mail: 
 > Created Time: 二 12/26 12:04:08 2017
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){

    int i;
    pid_t pid;

    pid = fork();

    if(pid == 0){
        printf("pgid=%d\n", getpgid(getpid()));
        printf("pgrp=%d\n", getpgrp());
    }else{
        printf("pid=%d\n", getpid()); 
    }
    while(1);
    return 0;
}
