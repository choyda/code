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
    //ss
    char  str1[3];
    char  str2[]="C Languagessssssssssss";
    printf("%d\n", strlen(str1));
    strcpy(str1, str2);
    puts(str1);
    puts(str2);
    printf("%d\n", strlen(str1));
    return 0;
//    char str[10] = "sssssssssssssssssss";
    char str[10];
    strcpy(str, "hahahahahahhhsssssssssss");
    printf("%s", str);

}
