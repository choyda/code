/*************************************************************************
 > File Name: setitimer1.c
 > Author: 
 > Mail: 
 > Created Time: 四 12/21 09:28:18 2017
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
//模仿 alarm函数，1秒钟打印输出
unsigned int my_alarm(int sec){
    
    struct itimerval newtime, oldtime;
    //四个成员都要设置
    newtime.it_value.tv_sec = 1;//设置1秒
    newtime.it_value.tv_usec = 0;
    newtime.it_interval.tv_sec = 0;
    newtime.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &newtime, &oldtime);
    return oldtime.it_interval.tv_sec;
}

int main(){
    int i=0;
    unsigned int alarm_ret;
    my_alarm(1);
    while(1){
        printf("%d\n", i++);
    }
    return 0;
}
