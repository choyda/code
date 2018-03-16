/*************************************************************************
 > File Name: fork.c
 > Author: 
 > Mail: 
 > Created Time: 一 12/18 17:00:44 2017
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>

int main(){

    pid_t pid;
    pid = fork();
    pid = fork();

    if(pid == 0){
        printf("xxxxxx\n");
    }
    if(pid > 0){
        sleep(1);
        printf("fffffffff\n");
    }
    return 0;
}
