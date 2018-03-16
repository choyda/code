/*************************************************************************
  > File Name: err_wrap.c
  > Author: 
  > Mail: 
  > Created Time: 三  1/24 14:58:24 2018
 ************************************************************************/

//错误处理函数
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
void err_pexit(const char * s){
    perror(s);
    exit(1);
}

int Socket(int domain, int type, int protocol){
    int r;
    if((r = socket(domain, type, protocol) < 0)){
        err_pexit("socket err");
    }
    return r;
}

int Bind(int fd, const struct sockaddr *sa, socklen_t salen)
{
    int n;
    if ((n = bind(fd, sa, salen)) < 0)
        err_pexit("bind error");
    return n;
}

int Listen(int socket, int backlog){
    int r;
    if(( r = listen(socket, backlog)) < 0){
        err_pexit("listen err");
    }
    return r;
}

int Connect(int fd, const struct sockaddr *sa, socklen_t salen)
{
    int r;
    if ((r = connect(fd, sa, salen)) < 0)
        err_pexit("connect error");

    return r;
}


int Accept(int socket, struct sockaddr * address, socklen_t * address_len){
    int r;
again:
    if((r =  accept(socket, address, address_len)) < 0){
        if ((errno == ECONNABORTED) || (errno == EINTR)){
            goto again;
        }else{
            err_pexit("accept err");
        }
    }
    return r;

}


ssize_t Read(int fd, void *ptr, size_t nbytes)
{
    ssize_t n;

again:
    if ( (n = read(fd, ptr, nbytes)) == -1 ) {
        if (errno == EINTR)
            goto again;
        else
            return -1;

    }
    return n;

}

ssize_t Write(int fd, const void *ptr, size_t nbytes)
{
    ssize_t n;

again:
    if ( (n = write(fd, ptr, nbytes)) == -1 ) {
        if (errno == EINTR)
            goto again;
        else
            return -1;

}
    return n;

}

int Close(int fd)
{
    int n;
    if ((n = close(fd)) == -1)
        err_pexit("close error");

    return n;

}
