/*************************************************************************
  > File Name: pthread_for.c
  > Author: 
  > Mail: 
  > Created Time: 四 12/28 09:33:24 2017
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void * fun_ex(){
    pthread_exit((void *)1);
}

void * pth_fun(void * num){
    long i = (long)num;
    if(i == 2){
        fun_ex(); //函数嵌套，也是退出线程。
    }
    printf("In pthread %ld, pt_id %llu\n", i, pthread_self());
   //exit(1); //退出整个进程
    return 0; //返回主控函数调用处 
}

int main(int argc, char *argv[], char *env[]){
    //long 类型的i ，如果用int的话， void * 强制转换会报警告， 原因是小地址转高地址。
    char *pth_err;
    long i, pth_ret;
    pthread_t pth_id;
    for(i=0; i<5; i++){
        pth_ret = pthread_create(&pth_id, NULL, pth_fun, (void *) i);
        if(pth_ret != 0){
            pth_err = strerror(pth_ret);
        }
    }
    printf("In main pthread %llu\n", pthread_self());
    pthread_exit((void *) 1); //表示退出主控线程，不会影响子线程的执行，所以子线程都执行完才结束。 }
}
