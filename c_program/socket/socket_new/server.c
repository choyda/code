/*************************************************************************
  > File Name: server.c
  > Author: 
  > Mail: 
  > Created Time: 三  1/24 15:55:56 2018
 ************************************************************************/

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <ctype.h>
#include "err_wrap.h"
#include <arpa/inet.h>
#include <string.h>
#include <sys/signal.h>


#define SERV_PORT 9998

void wait_pid(int signo){
    while(waitpid(0, NULL, WNOHANG) > 0);
    return ;
}

int main(int argc, char * argv[]){

    int sfd, cfd;
    struct sockaddr_in serv_addr, cli_addr;
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int opt = 1;
    // sockfd为需要端口复用的套接字
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));
    Bind(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    Listen(sfd, 64);
    socklen_t cli_len;
    cli_len = sizeof(cli_addr);
    pid_t pid;
    while(1){
        cfd = Accept(sfd, (struct sockaddr *)&cli_addr, &cli_len);
        pid = fork();
        if(pid == 0){
            Close(sfd);
            char buf[BUFSIZ];
            int n, i;
            while(1){
                n = Read(cfd, buf, sizeof(buf));
                Write(STDOUT_FILENO, buf, n);
                for(i=0;i<n;i++){
                    buf[i] = toupper(buf[i]);
                }
                sleep(10);
                Write(cfd, buf, n);
            }
        }else if(pid > 0){
            signal(SIGKILL, wait_pid);
            Close(cfd);
        }
    }
    return 0;

}

