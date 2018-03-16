/*************************************************************************
  > File Name: pthread_cancel.c
  > Author: 
  > Mail: 
  > Created Time: 四 12/28 17:54:16 2017
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>


void * pt_fun1(void * i){
    pthread_exit((void *) 111);
}

void * pt_fun2(void * i){
    return (void *) 222;
}

void * pt_fun3(void * i){
    while(1){
        //    sleep(1);
        //   printf("###\n");
        pthread_testcancel(); //设置取消点
    }
    return (void *) 333;
}
int main(){
    pthread_t pt_id;
    void *i;

    pthread_create(&pt_id, NULL, pt_fun1, NULL);
    pthread_join(pt_id, &i);
    printf("--------@%d\n", (int)i);

    pthread_create(&pt_id, NULL, pt_fun2, NULL);
    pthread_join(pt_id, &i);
    printf("--------@%d\n", (int)i);

    pthread_create(&pt_id, NULL, pt_fun3, NULL);
    sleep(3);
    pthread_cancel(pt_id);
    pthread_join(pt_id, &i);
    printf("--------@%d\n", (int)i);

    return 0;
}
