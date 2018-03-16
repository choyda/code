/*************************************************************************
 > File Name: fork_for1.c
 > Author: 
 > Mail: 
 > Created Time: 一 12/18 17:13:27 2017
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>

int main(){

    int i;
    pid_t pid;
    //父进程睡眠一秒的意思，第二次fork在1秒之后， 也就是在一秒内， 子进程输出，
    //1秒后父进程输出， 然后在fork，然后第二次子进程输出，1秒后父进程输出，然后再fork。
    //循环下去....., 输出效果就是交替打印
    //
    //如果不睡眠，相当于内核在做for的时候会检查for里有两次fork，
    //然后会同时fork2次，相当于同时产生2个子进程。也就是4个进程竞争cpu使用权，
    //感觉就没有先后打印顺序里。
    for(i=0; i<2; i++){
        pid = fork();
        if(pid == 0){
            printf("xxxxxxxxxxxxxxx\n");
            break;
        }
        if(pid > 0){
            sleep(1);
            printf("yyyyyyyyyyyyyy\n");
        }

    }
}
