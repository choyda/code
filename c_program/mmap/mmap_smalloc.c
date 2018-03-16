/*************************************************************************
 > File Name: mmap_smalloc.c
 > Author: 
 > Mail: 
 > Created Time: 一 12/11 09:37:20 2017
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#define MAP_SIZE 8
int *smalloc(int s_size){
    int *mp = mmap(NULL, MAP_SIZE, PROT_WRITE|PROT_READ,MAP_SHARED|MAP_ANON, -1, 0);
    if(mp == MAP_FAILED){
        perror("mmap error");
        exit(1);
    }
    return mp;

}
void sfree(int *p){
    int is_mun = munmap(p, MAP_SIZE);
    if(is_mun == -1){
        perror("munmap error");
        exit(1);
    }     
}

int main(int argc, char ** argv, char ** env){
    
    int *p;
    p = smalloc(MAP_SIZE);
    pid_t pid = fork();
    
    if(pid==0){
        *p = 1234;
    }else{
        sleep(1);
        printf("我是父进程%d\n", *p);
    }

    sfree(p);

    return 0;
}
