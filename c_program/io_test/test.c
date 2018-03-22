/*************************************************************************
 > File Name: test.c
 > Author: 
 > Mail: 
 > Created Time: 四 12/28 09:07:24 2017
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

ssize_t cd_readn(int fd, void *vptr, size_t n) {

    size_t  nleft;                  //usigned int 剩余未读取的字节数
    ssize_t nread;                  //int 实际读到的字节数
//    char   *ptr = (char *)vptr;     //指向缓冲区的指针

    nleft = n;                      //指定要读的字节数 比如5000

    printf("ss:%d\n", sizeof(vptr));
    printf("aa:%d\n", strlen(vptr));
    while (nleft > 0) { //5000大于0, 第二次循环nleft=4000
        if ((nread = read(fd, vptr, nleft)) < 0) { //ptr += nread表示下次读的时候按着+完的位置放
            if (errno == EINTR) {   //出错逻辑
                nread = 0;          //EINTR错误，重读，表示一个自己没读到
            }else {
                return -1;
            }
        }else if(nread == 0){       //如果读完了，就返回
            break;
        }
        nleft -= nread;             //1，如果是EINTR的0，相当于没读到，重新读
        //2，比如读到1000，用5000-1000，还剩4000没读
        printf("dd:%d", nread);
        printf("ee:%s\n", vptr);
//        vptr += nread;               //后移指针1000的位置， 下次从这个位置读
    }
    return n - nleft;               //1，一次读完直接就是5000 -5000 返回0，在调用方表示读完。
    //2,如果5000，一次1000，最后2buf次，一次是800，一次是600，多出4000
    //传进来的vpte大小是5000，会自动截取后面的。
}

int main(){

    int fd, rd;
    char buf[3]="abc";
    fd = open("rewr.txt", O_RDONLY);
    rd = read(fd, buf, 15);
    printf("rdd:%d\n", rd);
    printf("buf:%s\n", buf);


//    printf("%s\n", aa);

//    char buf[5];
//    int rd;
//    rd = cd_readn(fd, buf, 15);

//    printf("ff:%s\n", buf);

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
