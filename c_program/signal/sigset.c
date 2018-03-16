/*************************************************************************
 > File Name: sigset.c
 > Author: 
 > Mail: 
 > Created Time: 四 10/19 10:15:54 2017
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void printSigPending(sigset_t *pend){
    int i;
    for(i=1; i<=32;i++){
        if(sigismember(pend, i) == 1){
            putchar('1');
        }else{
            putchar('0');
        }
    }

}
int main(int argc, char *argv[]){
    
    sigset_t myset, oldset, pending;

    sigemptyset(&myset);
    sigaddset(&myset, SIGKILL);
    //sigaddset(&myset, SIGQUIT);
    sigaddset(&myset, SIGINT);
    sigaddset(&myset, SIGTSTP);
    sigprocmask(SIG_BLOCK, &myset, &oldset);
    while(1){
        sigpending(&pending);
        printSigPending(&pending);
        printf("\n");
        sleep(1);
    }
    return 0;
}
