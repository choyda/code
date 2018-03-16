/*************************************************************************
  > File Name: server2.c
  > Author: 
  > Mail: 
  > Created Time: 二  1/23 17:55:04 2018
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>

#define SOCK_PORT 6666
#define SIZEBUF 1024
int main(int argc, char * argv[], char * env[]){

    int sfd, cfd;
    //1, socket
    sfd = socket(AF_INET, SOCK_STREAM, 0);

    //2,bind
    socklen_t socklen;
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SOCK_PORT);
    bind(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));// 注意绑定的参数,最后一个参数，导致客户端连接不上。

    //3, listen
    listen(sfd, 64);

    //4,accept
    struct sockaddr_in cli_addr;
    socklen_t cli_len;
    cli_len = sizeof(cli_addr);
    cfd = accept(sfd, (struct sockaddr *)&cli_addr, &cli_len);

    //read,write
    char buf[SIZEBUF];
    int len, i;
    while(1){
        len = read(cfd, buf, sizeof(buf));
        write(STDOUT_FILENO, buf, len);

        for(i=0; i<len; i++){
            buf[i] = toupper(buf[i]);
        }
        write(cfd, buf, len);//write 最后一个参数，导致写入的是乱码
    }

    close(sfd);
    close(cfd);
    return 0;
}
