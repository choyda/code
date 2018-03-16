/*************************************************************************
 > File Name: dup2.c
 > Author: 
 > Mail: 
 > Created Time: 四  9/28 21:46:47 2017
 ************************************************************************/

#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char **argv){

    int fd;
    fd = open("dup2.txt", O_WRONLY|O_CREAT|O_TRUNC, 0644);
    if(dup2(fd, 1) ==-1){ //把程序的标准输出，从1终端屏幕指到打开的文件中。
        printf("error dup2");
    }

    if(execlp("ps", "ps", "aux", NULL)==-1){
        printf("execlp error");
    }
    return 0;
}
