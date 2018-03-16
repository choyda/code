/*************************************************************************
 > File Name: inet_pton.c
 > Author: 
 > Mail: 
 > Created Time: 六  1/27 22:50:55 2018
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
int main (void)
{
  char IPdotdec[20]; // 存放点分十进制IP地址
  struct in_addr s;  // IPv4地址结构体
  // 输入IP地址
  printf("Please input IP address: ");
  scanf("%s", IPdotdec);
  // 转换
  inet_pton(AF_INET, IPdotdec, (void *)&s);
  printf("inet_pton: 0x%x\n", s.s_addr); // 注意得到的字节序
  // 反转换
  inet_ntop(AF_INET, (void *)&s, IPdotdec, 16);
  printf("inet_ntop: %s\n", IPdotdec);
}
