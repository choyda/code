/*************************************************************************
 > File Name: pause.c
 > Author: 
 > Mail: 
 > Created Time: 四 12/21 16:27:10 2017
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

//利用alarm和pause函数实现sleep函数的功能。
void catchfun(int signo){
}

void mysleep(int sec){
    signal(SIGALRM, catchfun);
    alarm(sec);
    pause();
}

int main(){
    mysleep(1);
    printf("xxxxxx\n");
    alarm(1); //主要要恢复alarm的默认处理动作，用sigaction处理.
    while(1);
    return 0;
}
