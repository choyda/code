/*************************************************************************
  > File Name: sigpending.c
  > Author: 
  > Mail: 
  > Created Time: 四 12/21 10:22:00 2017
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void printset(sigset_t *printset){
    int i, is_sig;
    for(i=1; i<=32; i++){
        is_sig = sigismember(printset, i);
        if(is_sig){
            printf("%d", 1);
        }else{
            printf("%d", 0);
        }
    }
    printf("\n");
}

int main(int argc, char *arv[], char *env[]){
    //设置自己的信号集
    sigset_t set, old, setpend;
    sigemptyset(&set); //将自定义集合清空
    //sigfillset(&set); //将自定义集合都设置为1，与上面互斥.
    sigaddset(&set, SIGQUIT); //将3号信号添加到自定义信号集中
    //sigdelset(&set, SIGQUIT); //将3号信号从自定义集合中删除，与上面互斥。

    //同步关联系统信号集
    sigprocmask(SIG_BLOCK, &set, &old);
    while(1){
        //打印未决信号集
        sigpending(&setpend);
        printset(&setpend);
        sleep(1);
    }
    return 0;
}

