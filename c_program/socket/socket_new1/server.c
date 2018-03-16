/*************************************************************************
 > File Name: server.c
 > Author: 
 > Mail: 
 > Created Time: 一  1/29 14:41:31 2018
 ************************************************************************/

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <strings.h>
#include <arpa/inet.h>
#include <ctype.h>
#define SOCK_PORT 6666
#define SOCK_IP "127.0.0.1"
int main(int argc, char * argv[]){
    
    int sfd, cfd;
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sock_serv, sock_clin;
    bzero(&sock_serv, sizeof(sock_serv));
    sock_serv.sin_port = htons(SOCK_PORT);
    sock_serv.sin_family = AF_INET;
    inet_pton(AF_INET, SOCK_IP, (void *)&sock_serv.sin_addr.s_addr);
    //sock_serv.sin_addr.s_addr = inet_pton(AF_INET, SOCK_IP, (void *)&s); //绑定ip方法
    //sock_serv.sin_addr.s_addr = htonl(INADDR_ANY); //绑定ip方法 
    socklen_t sock_len;
    
//    int opt = 1;
//    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    bind(sfd, (struct sockaddr *) &sock_serv, sizeof(sock_serv));

    listen(sfd, 64);
    
    socklen_t sock_clin_len;
    sock_clin_len = sizeof(sock_clin);
    cfd = accept(sfd, (struct sockaddr *)&sock_clin, &sock_clin_len);

    char buf[200];
    int n, i;
    while(1){
       n = read(cfd, buf, sizeof(buf));
       write(STDOUT_FILENO, buf, n);
       for(i=0;i<n;i++){
            buf[i] = toupper(buf[i]);
       }
       write(cfd, buf, n);
    } 

    close(cfd);
    close(sfd);
}
