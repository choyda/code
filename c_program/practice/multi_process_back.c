/*************************************************************************
  > File Name: multi_process.c
  > Author: 
  > Mail: 
  > Created Time: 一 12/11 11:00:38 2017
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

int main(int arvc, char ** argv, char ** env){
    int pro_num =3, i, w_cnt, y_fd, m_fd, y_len, r_cnt, ave, end;
    pid_t pid;
    char buf[1024];
    char *yp;
    char *mp;
    //源文件
    y_fd = open("multi_process.txt", O_RDONLY);
    if(y_fd == -1){
        perror("open yfd error");
        exit(1);
    }
    y_len = lseek(y_fd, 0, SEEK_END);//源文件长度
    yp = mmap(NULL, y_len, PROT_READ, MAP_SHARED, y_fd, 0); 
    if(yp == MAP_FAILED){
        perror("map_yp error");
        exit(1);
    }
    ave = y_len / pro_num;//每个进程copy的字节数
    end = y_len % (y_len/pro_num);//多出来的字节数
    close(y_fd);
    //目标文件
    m_fd = open("cp_process.txt", O_RDWR|O_CREAT|O_TRUNC,0644);
    ftruncate(m_fd, y_len);
    mp = mmap(NULL, y_len, PROT_WRITE|PROT_READ, MAP_SHARED, m_fd, 0);
    //循环子进程
    //printf("%d\n",ave);
    //printf("%d\n",y_len);
    //printf("%d\n",end);
    for(i=0; i<pro_num; i++){
        pid = fork();
        if(pid == -1){
            perror("pid error");
        }
        if(pid == 0){
            sleep(i);
            int wi;
            char wstr[1024];
            char ch[5];
            for(wi=0; wi<ave; wi++){
                *mp = *(yp+wi+i*ave);
                mp++;
            }
            if(0 && i==(pro_num-1) && end){
                for(wi =0;wi<end;wi++){
                    wstr[wi] = *(yp+wi+i*ave+ave);
                }
            }
            break;
        }
    }
    if(i == pro_num){
        sleep(i);
        printf("%s\n", mp);
        read(m_fd, buf, 1024);
        write(STDOUT_FILENO, buf, 1024);
    }
    return 0;
}
