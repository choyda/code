/*************************************************************************
 > File Name: test.c
 > Author: 
 > Mail: 
 > Created Time: 四 12/28 09:07:24 2017
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main(){

   const int a=100;
   int *p = &a;
   printf("%d\n", *p);
   *p=200;

   printf("%d\n", *p);
   return 0;
}
