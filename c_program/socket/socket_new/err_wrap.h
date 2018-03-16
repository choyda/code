/*************************************************************************
 > File Name: err_wrap.h
 > Author: 
 > Mail: 
 > Created Time: 三  1/24 15:59:06 2018
 ************************************************************************/

#ifndef _ERR_WRAP_H
#define _ERR_WRAP_H
//int Bind(int fds, const struct sockaddr *sa, socklen_t salen);
//int Bind(int socket, const struct sockaddr *address, socklen_t address_len);
void err_pexit(const char * s);
int Socket(int domain, int type, int protocol);
int Bind(int socket, const struct sockaddr *address, socklen_t address_len);
int Listen(int socket, int backlog);
int Connect(int fd, const struct sockaddr *sa, socklen_t salen);
int Accept(int socket, struct sockaddr *address, socklen_t *address_len);
ssize_t Read(int fd, void *ptr, size_t nbytes);
ssize_t Write(int fd, const void *ptr, size_t nbytes);
int Close(int fd);
#endif
