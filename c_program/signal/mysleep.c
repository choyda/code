/*************************************************************************
 > File Name: pause.c
 > Author: 
 > Mail: 
 > Created Time: 五 10/20 13:55:32 2017
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
void act_fun(signo){
    printf("捕获的内容...\n");
}

unsigned int mysleep(unsigned secounds){
    int ret;
    struct sigaction act, oldact; 
    act.sa_handler = act_fun;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGALRM);
    sigaddset(&act.sa_mask, SIGINT);
    act.sa_flags = 0;
    sigaction(SIGALRM, &act, &oldact);
    alarm(secounds); //睡眠1秒，失去cpu的时间内已经过了闹钟时间。
    //----------------------------------失去cpu，执行其他程序2秒。
    ret = pause(); //主动挂起，等待唤醒。 //如果失去cpu时间大于alarm的时间，就会永久挂起，不能唤醒。
    if(ret == -1){
        printf("pause 成功\n");
    }
    sigaction(SIGALRM, &oldact, NULL); //封装第三方函数库，用完函数捕捉需要把原来的系统的老的处理动作恢复回去，
    //要不然 40行的alarm 不起作用。
    ret = alarm(0); // 睡3秒的过程中，来了一个信号唤醒pause，这个是清除之前设置的信号，保持程序的健壮性。
    return ret;
}


int main(){
    mysleep(3);
    printf("睡3秒醒了...\n");
    alarm(1); //设置完捕捉函数要恢复默认动作，要不不起作用。
    while(1);
}
