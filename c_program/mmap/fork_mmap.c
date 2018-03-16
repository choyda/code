/*************************************************************************
 > File Name: fork_mmap.c
 > Author: 
 > Mail: 
 > Created Time: 五 12/ 8 11:39:01 2017
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#define MAP_SIZE 1024 
int main(int argc , char ** argv, char ** env){
    
    int fd, ft;
    int *p;
    pid_t pid;
    int var = 100;
    fd = open("fork_mmap.txt", O_RDWR | O_CREAT | O_TRUNC, 0644); 
    if(fd == -1){
        perror("open error");
        exit(1);
    }
    //删除临时文件目录项，使之具备被释放的条件。          
    //当调用这个文件描述符的进程全部结束了， 这个文件就释放了。
    //unlink的主要目的是提供父子进程通信的一个介质，通信完成就释放掉，不需要写入磁盘。
    //当所有调用这个文件描述符的进程结束后， 这个文件就被释放掉了
    unlink("fork_mmap.txt"); //如果不unlink 为什么munmap会报错???

    ft = ftruncate(fd, MAP_SIZE); // 创建文件的大小。
    if(ft == -1){
        perror("ftruncate error");
    }

    p = mmap(NULL, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    //p = mmap(NULL, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0); //如果是private 父子进程的mmap是独立的不是共享的。需要特别注意， 如果想共享使用，必须使用map_shared。
    if(p == MAP_FAILED){ // 注意p不是NULL
        perror("mmap error");
    }
    close(fd);
    pid = fork();
    if(pid == 0){
        *p = 20000; //p是指针 接受的是地址， *p是p指向地址的那个值.
        var = 1000; //父子进程 全局变量，读时共享，写时复制。
        printf("*p=%d, var=%d\n", *p, var);
    }else{
        sleep(1);
        printf("*p=%d, var=%d\n", *p, var);
        int mun = munmap(p, MAP_SIZE);
        if(mun == -1){
            perror("munmap error"); 
        }
    }

    return 0;
}
