/*************************************************************************
 > File Name: pthread_join.c
 > Author: 
 > Mail: 
 > Created Time: 四 12/28 11:33:02 2017
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int a;
    char *b;
    float c;
    char d;
} exit_t;

void * pt_fun(void * exit_arg){
    exit_t *arg = (exit_t *) exit_arg;
    arg->a = 100;
    arg->b = "caida";
    arg->c = 1.2;
    arg->d = 'd';
    pthread_exit((void *) exit_arg);
}

int main(void){

    pthread_t pt_id;
    int pt_ret;
    exit_t *exitret = (exit_t *) malloc(sizeof(exit_t));

    pt_ret = pthread_create(&pt_id, NULL, pt_fun, (void *) exitret);
    if(pt_ret !=0 ){
        fprintf(stderr, "%s\n", strerror(pt_ret));
    }

    //回收子线程的返回值阻塞等待，对应waitpid
    pthread_join(pt_id,(void **)&exitret);
    printf("%d\n", exitret->a);
    printf("%s\n", exitret->b);
    printf("%f\n", exitret->c);
    printf("%c\n", exitret->d);

    pthread_exit((void *) 9);
}
