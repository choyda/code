/*************************************************************************
 > File Name: hello.c
 > Author: 
 > Mail: 
 > Created Time: 四  9/28 21:36:13 2017
 ************************************************************************/

#include<stdio.h>
#include <unistd.h>
extern char ** environ;

int main(int argc, char **argv){
    int i;
    for(i=0; environ[i]!=NULL; ++i){
        printf("%s\n", environ[i]);
    }
    return 0;
}
