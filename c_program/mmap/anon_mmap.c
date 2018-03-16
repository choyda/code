/*************************************************************************
 > File Name: anon_mmap.c
 > Author: 
 > Mail: 
 > Created Time: 五 12/ 8 17:44:06 2017
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

int main(int argc, char ** argv, char ** env){
    //匿名映射区
    int *p;
    //p = mmap(NULL, 4, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);

    //第二种方法
    int fd = open("/dev/zero", O_RDWR);
    p = mmap(NULL, 8, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    pid_t pid = fork();

    if(pid == 0){
        *p = 1234;
        printf("p%d\n", *p);
    }else{
        sleep(1);
        printf("p%d\n", *p);
        munmap(p, 8);
    }
    
    return 0;
}
