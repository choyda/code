/*************************************************************************
 > File Name: getuid.c
 > Author: 
 > Mail: 
 > Created Time: 三  9/27 14:25:39 2017
 ************************************************************************/

#include<stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    uid_t uid;
    uid = getuid();
    printf("%d", uid);
}
