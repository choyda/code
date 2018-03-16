/*************************************************************************
 > File Name: alarm_time.c
 > Author: 
 > Mail: 
 > Created Time: 三 12/20 17:41:12 2017
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>


int main(){
    int i = 0, fd;
    fd = open("alarm.txt", O_RDWR|O_CREAT|O_TRUNC, 0644);
    alarm(1);
    while(1){
        printf("%d\n", i++);
    }

    return 0;
}
