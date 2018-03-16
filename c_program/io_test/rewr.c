/*************************************************************************
 > File Name: rewr.c
 > Author: 
 > Mail: 
 > Created Time: 五  9/29 21:47:43 2017
 ************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]){

    char buff[12] = "012345678910";
    char rebuff[12];
    printf("%lu\n", sizeof(buff)); 
    int fd1,fd2,wr_cnt, re_cnt;

    //fd1 = open("rewr.txt", O_WRONLY | O_CREAT | O_TRUNC, 0664);
    fd1 = open("rewr.txt", O_RDONLY);
    printf("%d\n", fd1); 
    //wr_cnt = write(fd1, buff, 12);

    re_cnt = read(fd1, rebuff, 12);
    printf("re%d\n", re_cnt);
    printf("%s", rebuff);
    //printf("%d\n", wr_cnt);

}
