/*************************************************************************
 > File Name: signal.c
 > Author: 
 > Mail: 
 > Created Time: 四 12/21 11:05:46 2017
 ************************************************************************/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void catchsigfun(int signo){
    printf("signo%d\n", signo);
}

int main(){
    alarm(3);
    sig_t sig_int;
    sig_int = signal(SIGALRM, catchsigfun);
    if(sig_int == SIG_ERR){
        perror("signal error:");    
    }
    while(1);
}
