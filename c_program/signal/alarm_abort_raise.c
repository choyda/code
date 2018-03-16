/*************************************************************************
 > File Name: alarm.c
 > Author: 
 > Mail: 
 > Created Time: 三 10/18 22:56:44 2017
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <stdlib.h>
int main(){
    //abort(); //给自己发送一个6号SIGABRT信号，默认动作是终止进程。
    //raise(SIGKILL); //给自己发送一个指定的信息, 相当于 kill(getpid(), SIGKILL);
    alarm(3); // 定时器 3秒钟后，给自己发送一个14号SIGALRM信号，默认终止进程。
    int i=0;
    printf("%s\n","定时打印,3秒后发送14号信号");
    for(;;);
    return 0;
}
