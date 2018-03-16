/*************************************************************************
 > File Name: server.c
 > Author: 
 > Mail: 
 > Created Time: 五 11/ 3 16:05:36 2017
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include "wrap.h"

#define SERV_PORT 6666
#define BUF_SIZE 2048
int main(void){

    int lfd, cfd;//连接的文件描述符号
    struct sockaddr_in serv_addr, clie_addr;

    //1, 创建socket客户端
    lfd = Socket(AF_INET, SOCK_STREAM, 0);

    //2, bind(int socket, const struct sockaddr *address, socklen_t address_len);
	bzero(&serv_addr, sizeof(serv_addr));           
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    Bind(lfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    
    //3, listen
    listen(lfd, 128);
    char clie_IP[BUF_SIZE];
    socklen_t clie_addr_len;
    clie_addr_len = sizeof(clie_addr_len);
    cfd = Accept(lfd, (struct sockaddr *)&clie_addr, &clie_addr_len);
    printf("client IP:%s\tport:%d\n", 
            inet_ntop(AF_INET, &clie_addr.sin_addr.s_addr, clie_IP, sizeof(clie_IP)), 
            ntohs(clie_addr.sin_port));

    int i, n;
    char buf[BUF_SIZE];
    while(1){
        n = read(cfd, buf, sizeof(buf));
        write(STDOUT_FILENO, buf, n);
        for(i = 0; i<n; i++){
            buf[i] = toupper(buf[i]);
        }
        write(cfd, buf, n);
    }
    close(lfd);
    close(cfd);
    return 0;
}
