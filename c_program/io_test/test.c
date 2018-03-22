/*************************************************************************
 > File Name: test.c
 > Author: 
 > Mail: 
 > Created Time: 四 12/28 09:07:24 2017
 ************************************************************************/

#include <stdio.h>
#include <string.h>

int main(){

    char *token;
    char RxBuffer[1024];
    token = strtok(RxBuffer, "\n");

//    char c = 129;
//    unsigned char a=200; //a==200
//    unsigned char b=100;  //b == 100
//    unsigned char c=a+b;  //c ==300, 但是存不下就要扩展
//    printf("%d,%d",a+b,c);


//    printf("c=%d\n", c);
//    return 0;
//    signed char a =1;
//    unsigned char b = 1;
//    char c = 1;
//
//    printf("%d", a);
//    printf("%d", b);
//    printf("%d", c);
//
//
//    unsigned char RxBuffer[2][1024];
//    memset(RxBuffer, '\0', sizeof(RxBuffer) - 1);
}
