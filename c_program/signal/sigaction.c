/*************************************************************************
 > File Name: sigaction.c
 > Author: 
 > Mail: 
 > Created Time: 四 12/21 14:07:58 2017
 ************************************************************************/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void catch_act(int signo){
    sleep(5);
    printf("catch----------catch%d\n", signo);
}

int main(){   
    //注册捕捉函数开始
    struct sigaction sigact;
    sigact.sa_handler = catch_act; //捕捉函数
    sigemptyset(&sigact.sa_mask); //清空信号集
    sigaddset(&sigact.sa_mask, SIGTSTP); //添加信号集阻塞信号。
    sigact.sa_flags = 0;
    sigaction(SIGQUIT, &sigact, NULL );
    //注册捕捉信号完毕
    //1，先安ctrl+\ 进入到捕捉函数中。
    //2，睡眠5秒，期间再按ctrl+\， 会先打印一遍，然后在处理一次ctrl+\。
    //3, 如是是按ctrl+c,直接退出， 如果把ctrl+c阻塞住了，等睡眠5秒之后在执行这个信号。 
    while(1);
    return 0;
}
