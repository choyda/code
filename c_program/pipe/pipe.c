/*************************************************************************
  > File Name: pipe.c
  > Author: 
  > Mail: 
  > Created Time: 四 12/ 7 14:20:08 2017
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>

int main(int argc, char ** argv, char ** env){

    int fd[2];   
    int pipe_ret, pid;
    ssize_t w_size, r_size;
    pipe_ret = pipe(fd); //fd[0] --r, fd[1] --w;
    if(pipe_ret == -1){
        perror("pipe error:");
    }
    pid = fork();
    if(pid == -1){
        perror("fork error");
    }
    if(pid == 0){
        close(fd[1]); //关闭写端
        //子进程负责读
        char buf[1024];
        do{
            //5秒从管道读一次数据，然后管道中有空间， 写端继续写。
            sleep(5);
            r_size = read(fd[0], buf, sizeof(buf)); 
            if(r_size > 0){
                printf("一共读了%zd\n", r_size);
            }
            write(STDOUT_FILENO, buf, sizeof(buf));
        }while(r_size);
        if(r_size == 0){
            printf("管道中没有数据\n");
        }
    }
    if(pid > 0 ){
        close(fd[0]); //关闭读端
        //父进程写
        char buf[1024] ="管道中写的数据数据管道中写的数据数据管道中写的数据数据管道中写的数据数据end\n";
        do{ //如果管道写满了，write将阻塞等待读端，从管道读取数据在写入。可用ulimit -a查看
            w_size = write(fd[1], buf, sizeof(buf));
            if(w_size){
                printf("一共写入了%zd\n", w_size);
            }else{
                perror("write error:");   
            }        
        }while(w_size);

    }
}
