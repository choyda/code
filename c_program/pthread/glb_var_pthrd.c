/*************************************************************************
 > File Name: glb_var_pthrd.c
 > Author: 
 > Mail: 
 > Created Time: 四 12/28 10:25:09 2017
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

int var = 100;
void * pt_fun(void * arg){
    var = 200;
    return 0;
}
void * pt_fun1(void * arg){
    var = 300;
    return 0;
}
int main(){

    pthread_t pt_id;
    printf("var=%d\n", var);
    sleep(1);
    pthread_create(&pt_id, NULL, pt_fun, NULL);
    sleep(2);
    printf("var=%d\n", var);
    pthread_create(&pt_id, NULL, pt_fun1, NULL);
    sleep(3);
    printf("var=%d\n", var);

    return 0;
}
