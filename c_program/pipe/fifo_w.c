/*************************************************************************
 > File Name: fifo_w.c
 > Author: 
 > Mail: 
 > Created Time: 四 12/ 7 17:35:50 2017
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char ** argv, char ** env){

    int fifo_ret;
    int wfd, wsize;
    fifo_ret = mkfifo("myfifo", 0644);
    wfd = open("myfifo", O_WRONLY); 
    char *buf = "file写入的数据\n";
    wsize = write(wfd, buf, sizeof(buf));
    if(wsize == -1){
        perror("wsize error");
    }else if(wsize > 0){
        printf("写入%d个字节\n", wsize);
    }
    printf("%d\n", wsize);
    
    
    return 0;
}
