/*************************************************************************
  > File Name: pthread_loop_join.c
  > Author: 
  > Mail: 
  > Created Time: 四 12/28 14:19:46 2017
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

typedef struct{	
	int num;
	pthread_t pthread_id[6];
} exit_t;

void *pt_fun(void * index){
    long i = (long) index;	
	printf("In pthread_num %ld\n", i);
    pthread_exit((void *) i+10);
}

void *expt_fun(void * arg){
	exit_t *exit_arg_pt = (exit_t *) arg;
	int it;
	int retval;
	for(it=0; it<exit_arg_pt->num; it++){
		pthread_join(exit_arg_pt->pthread_id[it], (void **)&retval);
        printf("In chlid exit %d\n", retval);
	}
	pthread_exit((void *) 8);
}
int main(){
    pthread_t pt_id[6];
	pthread_t expt_id;
    int pt_ret[6];
	int expt_ret,ex_i;
    long i;
	exit_t *exit_arg = malloc(sizeof(exit_t)); //给结构体初始化，不能忘
    for(i=0; i<6; i++){
        pt_ret[i] = pthread_create(&pt_id[i], NULL, pt_fun, (void *)i);
        if(pt_ret[i] !=0 ){
            fprintf(stderr, "pthread_create error:%s\n", strerror(pt_ret[i]));  
        }
		exit_arg->pthread_id[i] = pt_id[i];
		exit_arg->num = i;
    }
	
    sleep(2);
    //创建子线程回收(循环多打印？没找到原因) 
    /*
	expt_ret = pthread_create(&expt_id, NULL, expt_fun, (void *)exit_arg);
    pthread_join(expt_id, (void **)&ex_i);
    printf("In main exit %d\n", ex_i);
    */ 
    //主控线程回收 没问题
    for(i=0; i<6; i++){
        pthread_join(pt_id[i], (void **)&ex_i);
        printf("In child exit %d\n", ex_i);
    }  
	pthread_exit((void * ) 9);
}
