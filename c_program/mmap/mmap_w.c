/************************************************************************* > File Name: mmap_w.c > Author: 
 > Mail: 
 > Created Time: 一 12/11 09:08:36 2017
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(int argc, char ** argv, char ** env){

    int fd;
    int *p;
    fd = open("mmap_wr.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    ftruncate(fd, 8); 
    p = mmap(NULL, 8, PROT_WRITE, MAP_SHARED, fd, 0); close(fd);
    if(p == MAP_FAILED){
        perror("mmap error");
    }
    *p = 123456;
    printf("我已经写完了....\n");

    return 0;
}
