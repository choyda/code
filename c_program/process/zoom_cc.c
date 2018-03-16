/*************************************************************************
 > File Name: zoom.c
 > Author: 
 > Mail: 
 > Created Time: 四  9/28 22:58:36 2017
 ************************************************************************/

#include<stdio.h>
#include <unistd.h>
#include <sys/wait.h>
int main(){

    pid_t pid,wpid;
    int i = 1;
    pid =fork();

    if(pid == 0){
        printf("child ----- die");
        sleep(5);
    }else{
        while(1){
            //i++;
            sleep(1);
            printf("parent --- ing...\n");  
            //if(i == 10){
              //  wpid = wait(NULL);
                //printf("%d", wpid);
            //    break;
            //}
        }
    }


    return 0;
}
