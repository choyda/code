#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <ctype.h>
#include <unistd.h>

#include "wrap.h"

#define MAXLINE 8192
#define SERV_PORT 8000

void do_sigchild(int num)
{
    while (waitpid(0, NULL, WNOHANG) > 0)
        ;
}

int main(void)
{
    struct sockaddr_in servaddr, cliaddr;
    socklen_t cliaddr_len;
    int listenfd, connfd;
    char buf[MAXLINE];
    char str[INET_ADDRSTRLEN];
    int i, n;
    pid_t pid;
    struct sigaction newact;

    newact.sa_handler = do_sigchild;
    sigemptyset(&newact.sa_mask);
    newact.sa_flags = 0;
    sigaction(SIGCHLD, &newact, NULL);

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    Listen(listenfd, 20);

    printf("Accepting connections ...\n");
    while (1) {
        cliaddr_len = sizeof(cliaddr);
        connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
        pid = fork();
        if (pid == 0) {
            Close(listenfd);
            while (1) {
                n = Read(connfd, buf, MAXLINE);
                if (n == 0) {
                    printf("the other side has been closed.\n");
                    break;
                }
                printf("received from %s at PORT %d\n",
                        inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
                        ntohs(cliaddr.sin_port));

                for (i = 0; i < n; i++)
                    buf[i] = toupper(buf[i]);

                Write(STDOUT_FILENO, buf, n);
                Write(connfd, buf, n);
            }
            Close(connfd);
            return 0;
        } else if (pid > 0) {
            Close(connfd);
        }  else
            perr_exit("fork");
    }
    return 0;
}

//
//阻塞：就是read()函数， 什么都不干，就等2秒，数据返回。
//非阻塞 ：比如调用read()函数， 需要2秒读完数据， 但是马上返回。
//
//同步：请求方(调用方)主动去查看结果。
//异步：请求方(调用方)被动获得结果。
//
//同步阻塞：调用方等待read()返回， 比如fork()出来的子进程等待。
//同步非阻塞：就是需要我用个for((n=read()) > 0)  去轮询read()函数是不是完事了。
//
//
//异步阻塞：
//异步：就是我需要注册一个回调函数，当read() 2秒完事了， 自动调用我的回调函数，处理读出来的数据。


