/*************************************************************************
 > File Name: open.c
 > Author: 
 > Mail: 
 > Created Time: 二 10/17 21:45:45 2017
 ************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
int main(){

    int fd;
    //打开一个不存在的文件
    fd = open("caida.txt", O_WRONLY | O_CREAT, 0777); 
    if(fd == -1){
        perror("open error:");
        exit(1);
    }

    close(fd);
    
}
