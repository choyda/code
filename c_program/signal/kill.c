/*************************************************************************
  > File Name: kill.c
  > Author: 
  > Mail: 
  > Created Time: 一 12/18 16:25:30 2017
 ************************************************************************/

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[], char *env[]){

    pid_t pid, kpid;
    int kret, i;

    //for 5次 fork创建子进程，相当于内核同时创建5个子进程
    for(i=0; i<5; i++){
        pid = fork();
        if(pid == 0){
            break; //子进程break，只执行一次循环体内的逻辑， 不会在进行for循环了。
        }
        if(i == 2){
            kpid = pid;
        }
    }
    if(pid == 0){ 
        //1,如果是子进程就循环一直打印自己的id，然后睡眠一秒，
        //2,也就是在父进程睡眠的1秒钟内， 5个子进程都执行了打印，然后进入睡眠。
        //3,子进程睡眠1秒醒了在次打印， 再睡眠。
        while(1){
            printf("i am child i=%d pid=%d\n", i, getpid());
            sleep(1);
        }
    }else{ 
        //1,如是父进程就睡眠一秒，交出cpu
        //2,在子进程都打印完睡眠的时候，父进程醒来kill子进程，然后一直阻塞。 
        sleep(1);
        kill(kpid, SIGKILL);        
        while(1);
    }

    return 0;
}
