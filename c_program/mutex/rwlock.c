/*************************************************************************
 > File Name: rw_mutex.c
 > Author: 
 > Mail: 
 > Created Time: 四  1/18 16:07:25 2018
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
int cun = 100;
pthread_rwlock_t rwlock;
void * rfun(void * argv){
    int i = (int) argv;
    while(1){
        pthread_rwlock_rdlock(&rwlock);
        printf("rrrr-----%d-------%lu------%d\n", i, (unsigned long)pthread_self(), cun);
        pthread_rwlock_unlock(&rwlock);
        sleep(2);
    }
    return NULL;
}

void * wfun(void * argv){
    int t, i = (int) argv;
    while(1){
        pthread_rwlock_wrlock(&rwlock);
        t = cun;
        usleep(1000);
        printf("wwww------%d-------%lu---%d---%d\n", i, (unsigned long)pthread_self(), t, ++cun);
        pthread_rwlock_unlock(&rwlock);
        sleep(1);
    }
    return NULL;
}

int main(int argc, char * argv[], char * env[]){
    pthread_rwlock_init(&rwlock, NULL);
    pthread_t tid[8];
    int i; 

    for(i=0; i<3; i++){
        pthread_create(&tid[i], NULL, rfun, (void *)i);    
        pthread_detach(tid[i]);
    }
    
    for(i=0; i<5; i++){
        pthread_create(&tid[i+3], NULL, wfun, (void *)i);    
        pthread_detach(tid[i+3]);
    }
    pthread_rwlock_destroy(&rwlock);
    while(1);
    return 0;
}
