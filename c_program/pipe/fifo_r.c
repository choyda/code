/*************************************************************************
 > File Name: fifo_r.c
 > Author: 
 > Mail: 
 > Created Time: 四 12/ 7 17:51:58 2017
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char ** argv, char ** env){

    int rfd, rsize;
    rfd = open("myfifo", O_RDONLY);
    char buf[1024];
    rsize = read(rfd, buf, sizeof(buf));
    if(rsize == 0){
        printf("读完了\n");
    }
    if(rsize > 0){
        printf("一共读了%d\n", rsize);
    }
    if(rsize == -1){
        perror("rsize error:");
    }
    return 0;
}
