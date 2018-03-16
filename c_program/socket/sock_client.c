/*************************************************************************
  > File Name: sock_client.c
  > Author: 
  > Mail: 
  > Created Time: 二  1/23 21:25:45 2018
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#define SERV_PORT 6666
#define SERV_IP "127.0.0.1"
#define SIZEBUF 1024
int main(int argc, char * argv[], char * env[]){

    int cfd;
    struct sockaddr_in serv_addr;
    cfd = socket(AF_INET, SOCK_STREAM, 0);
    socklen_t serv_addr_len;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, SERV_IP, &serv_addr.sin_addr.s_addr);
    connect(cfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    int n; 
    char buf[SIZEBUF];
    while(1){
        fgets(buf, sizeof(buf), stdin);
        write(cfd, buf, strlen(buf));
        n = read(cfd, buf, sizeof(buf));
        write(STDOUT_FILENO, buf, n);
    }
    close(cfd);
    return 0;
}
