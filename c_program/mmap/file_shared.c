/*************************************************************************
 > File Name: file_shared.c
 > Author: 
 > Mail: 
 > Created Time: 五 12/ 8 10:52:49 2017
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
int main(int argc, char ** argv, char **env){

    int fd, rint, wint;
    pid_t pid;
    fd = open("file_shared.txt", O_RDWR|O_CREAT|O_TRUNC, 0644);
    pid = fork();
    if(pid == 0){
        //子id读
        sleep(1);
        char buf[1024];
        lseek(fd, 0 ,SEEK_SET);//注意，共享的时候要把文件偏移量初始化到最前面。
        rint = read(fd, buf, sizeof(buf));
        write(STDOUT_FILENO, buf, rint);
    }else{
        //父 id写，  注意sizeof和strlen的区别，write如果用sizeof会出错。
        char * buf ="abc3333de";
        char *strr[4096];
        wint = write(fd, buf, strlen(buf));
        printf("%lu\n", sizeof(buf));
        printf("%lu\n", sizeof(strr));
        printf("%lu\n", strlen(buf));
        printf("%d\n", wint); 
    }

}
