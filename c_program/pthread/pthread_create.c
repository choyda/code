/*************************************************************************
 > File Name: pthread.c
 > Author: 
 > Mail: 
 > Created Time: 二 10/24 16:08:31 2017
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

//注意编译的时候要加上gcc pthread_create.c -pthread -o pthread_create
void * pthread_fun( void * p){
    printf("In pthread: pthread no %lu, pid %d\n", pthread_self(), getpid());
    return 0;
}

int main(){
    pthread_t ptid;
    int pth_ret;
    //不调用pthread_create 进程也会有线程id。默认一个进程包含至少一个线程。
    printf("In main1: pthread no %lu, pid %d\n", pthread_self(), getpid());
    pth_ret = pthread_create(&ptid, NULL, pthread_fun, NULL); 
    if(pth_ret !=0){ //线程错误号直接返回，不是存在error中，所有的线程都用这样的方式处理线程错误
        fprintf(strerr, "pthread_create error:%s\n", strerror(pth_ret));
    }
    sleep(1);    
    printf("In main2: pthread no %lu, pid %d\n", pthread_self(), getpid());

    return 0;
}
