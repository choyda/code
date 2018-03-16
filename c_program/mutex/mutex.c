/*************************************************************************
 > File Name: mutex.c
 > Author: 
 > Mail: 
 > Created Time: 三  1/17 16:39:08 2018
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
//多线程加锁交替打印。
pthread_mutex_t mutex;
pthread_mutex_t mutex_a;
void *tfun(void * argv){
    srand(time(NULL));
    while(1){
        pthread_mutex_lock(&mutex_a);
        printf("HELLO");
        sleep(rand() % 3);
        printf("WORLD\n");
        pthread_mutex_unlock(&mutex_a);
        sleep(rand() % 3);
    }
    return NULL;
}

int main(int argc, char * argv[]){
    int flg = 5;
    pthread_t tid;
    srand(time(NULL)); //改变随机数的种子
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_a, NULL); //死锁拥有a锁，同时获取b锁
    pthread_create(&tid, NULL, tfun, NULL);
    while(flg--){
        pthread_mutex_lock(&mutex);
        printf("hello");
        sleep(rand() % 3);
        printf("world\n");
        pthread_mutex_lock(&mutex_a);//死锁拥有a锁，同时获取b锁
        //pthread_mutex_lock(&mutex);//在没有解锁之前再次加锁。
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 3);
    }
    pthread_mutex_destroy(&mutex);
    pthread_cancel(tid);
    pthread_join(tid, NULL);
    return 0;
}

//线程a有一把锁，线程b有一把锁， 线程a锁的同时，又获取线程b的锁，就会造成死锁。
