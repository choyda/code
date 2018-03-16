/************************************************************************* > File Name: mmap_r.c
 > Author: 
 > Mail: 
 > Created Time: 一 12/11 09:23:58 2017
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char ** argv, char ** env){
    
    int fd;
    int *p;
    fd = open("mmap_wr.txt", O_RDONLY);
    p = mmap(NULL, 8, PROT_READ, MAP_SHARED, fd, 0);

    if(p == MAP_FAILED){
        perror("mmap error");
    }

    printf("我读的共享数据\n%d", *p);


    return 0;
}

