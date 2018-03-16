/*************************************************************************
 > File Name: multi_process.c
 > Author: 
 > Mail: 
 > Created Time: 二 12/12 08:57:06 2017
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

void sys_err(char *er);

int main(int argc, char *argv[])
{
    int src_fd, dst_fd;
    int file_length, per_length=4096, last_length, num, i, lsek=0;
    char *src_p, *dst_p;
    struct stat file_stat;
    pid_t pid;

    if(argc < 3)
    {
        printf("please input ./a.out source_file copy_file\n");
        exit(1);
    }

    if((src_fd = open(argv[1], O_RDWR)) == -1)
    {
        sys_err("open error");
    }
    if((dst_fd = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644)) == -1)
    {
        sys_err("open error");
    }

    if((stat(argv[1], &file_stat)) == -1)
    {
        sys_err("stat error");
    }
    file_length = file_stat.st_size;
    printf("file size if %d\n", file_length);
    num = file_length/per_length + 1;
    last_length = file_length % per_length;

    if((ftruncate(dst_fd, file_length)) == -1)
    {
        sys_err("ftruncate error");
    }

    for(i=0; i<num; i++)
    {
        pid = fork();
        if(pid == 0)
        {
            printf("I'm %dth child process, lsek = %d\n", i+1, lsek);
            break;
        }
        else if(pid > 0)
        {
            sleep(1);
            lsek+=per_length;
            printf("I'm parent, lsek = %d\n", lsek);
        }
        else
        {
            sys_err("fork error");
        }
    }

    if(i < num)
    {
		printf("%d\n",i);
        if(lsek+per_length > file_length)
        {
            printf("last_size is %d\n", last_length);
            src_p = mmap(NULL, last_length, PROT_READ|PROT_WRITE, MAP_SHARED, src_fd, lsek);
            if(src_p == MAP_FAILED)
            {
                sys_err("last_src mmap error");
            }
            dst_p = mmap(NULL, last_length, PROT_READ|PROT_WRITE, MAP_SHARED, dst_fd, lsek);
            if(dst_p == MAP_FAILED)
            {
                sys_err("last_dst mmap error");
            }
            memcpy(dst_p, src_p, last_length);
            munmap(src_p, last_length);
            munmap(dst_p, last_length);

        }
        else
        {
            src_p = mmap(NULL, per_length, PROT_READ|PROT_WRITE, MAP_SHARED, src_fd, lsek);
            if(src_p == MAP_FAILED)
            {
                sys_err("src mmap error");
            }
            dst_p = mmap(NULL, per_length, PROT_READ|PROT_WRITE, MAP_SHARED, dst_fd, lsek);
            if(dst_p == MAP_FAILED)
            {
                sys_err("dst mmap error");
            }
            memcpy(dst_p, src_p, per_length);
            munmap(src_p, per_length);
            munmap(dst_p, per_length);
        }
    }

    close(src_fd);
    close(dst_fd);

    return 0;
}

void sys_err(char *er)
{
    perror(er);
    exit(1);
}
