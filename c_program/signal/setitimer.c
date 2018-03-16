/*************************************************************************
 > File Name: setitimer.c
 > Author: 
 > Mail: 
 > Created Time: 三 10/18 22:16:41 2017
 ************************************************************************/

#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>

void signalHandle(int signo){
	switch(signo){
		case SIGALRM:
			printf("心跳包吧\n");
			break;
	}

}

int main(int argc, char * argv[]){

	signal(SIGALRM, signalHandle);

    struct itimerval newtime, oldtime;

/* 参数结构体类型
struct itimerval {  
    struct timeval it_interval; //next value  为计时间隔 
    struct timeval it_value;    //current value  为延时时长 
}; 
	struct timeval {  
    time_t      tv_sec;         //seconds  秒
    suseconds_t tv_usec;        // microseconds 微妙 
}; 
*/
    //首次定时3.5秒发送一个SINLARM信号，然后每隔2.9秒发送一个信号。
	newtime.it_interval.tv_sec = 2; // 间隔1秒 ，设置0表示不设置间隔时间
	newtime.it_interval.tv_usec = 999999; // 秒数和微妙相加。微妙不能设置为1000000 = 1秒
	newtime.it_value.tv_sec = 3;	//延长5秒
	newtime.it_value.tv_usec = 500000;  //微妙

    setitimer(ITIMER_REAL, &newtime, &oldtime);
    for(;;);
    return 0;
}

