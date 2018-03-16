/************************************************************************* > File Name: mmap.c > Author: 
 > Mail: 
 > Created Time: 五 12/ 8 08:53:34 2017
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <string.h>
#define BUF_SIZE 1024
//1, 建立文件，读写权限要和映射的空间保持一直的权限。
//2，给文件设置大小，添加字节数。
//3，建立映射区，小心每个参数的使用及出错。
//4，判断mmap返回值， 返回的是映射内存的首地址。
//5，关闭文件描述符。不影响内存同步文件。
//6，使用指针对内存进行读写操作。
//7, 关闭映射区。
int main(int argc, char ** argv, char ** env){
    int fd;
    void * mmp;
    fd = open("mymmap", O_RDWR|O_CREAT|O_TRUNC, 0644);
    ftruncate(fd, 1024);
    mmp = mmap(NULL,BUF_SIZE,PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if(mmp == MAP_FAILED){
        perror("create mmap error");
        exit(1);
    }
    close(fd);
    char * str = "abc";
    strcpy(mmp, str);
    printf("%s", mmp);
    munmap(mmp, BUF_SIZE);

    return 0;
}
