/*************************************************************************
  > File Name: pthread_detach.c
  > Author: 
  > Mail: 
  > Created Time: 四 12/28 17:29:00 2017
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void * pt_fun(void * num){
    int n = 3;
    while(n--){
        sleep(1);
        printf("倒数%d\n", n);
    }
    pthread_exit(NULL);
}
int main(){
    pthread_t pt_id;
    int pt_ret;

#if 1 
    //设置创建线程的属性 
    pthread_attr_t attr;  //定义结构体
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr,  PTHREAD_CREATE_DETACHED);
    pthread_create(&pt_id, &attr, pt_fun, NULL);
#else 
    pthread_create(&pt_id, NULL, pt_fun, NULL);
    pthread_detach(pt_id); //注视掉就不会报错，打开分离线程会自动回收。不用在pthread_join
#endif
    pt_ret = pthread_join(pt_id, NULL);
    while(1){
        sleep(1);
        if(pt_ret !=0){
            fprintf(stderr, "pthread_join error:\n%s", strerror(pt_ret));
        }else{
            printf("没有错误\n");
        }
    }
    return 0;
}
