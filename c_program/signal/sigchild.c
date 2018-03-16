/*************************************************************************
  > File Name: sigchild.c
  > Author: 
  > Mail: 
  > Created Time: 一 12/25 09:49:12 2017
 ************************************************************************/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void do_child(int signo){
    int status;
    pid_t pid;
    //如果把while改成if？
    //一次进入捕捉函数处理n个信号。
    //在处理函数中，又死掉了几个子进程，如果用if就会漏掉几个信号
    //常规信号，不会记录死亡的次数。
    while(pid = waitpid(0, &status, WNOHANG) > 0){
        if(WIFEXITED(status)){ //正常结束
            printf("---------child%d exit = %d\n", pid, WEXITSTATUS(status));
        }else if(WIFSIGNALED(status)){//异常终止
            printf("child %d, err status = %d\n", pid,WTERMSIG(status));
        }
    }
}
void printped(sigset_t *ped){
    int i;
    for(i=1; i<=32; i++){
        if(sigismember(ped, i)){
            printf("%d",1);
        }else{
            printf("%d",0);
        } 
    }
}

int main(int argc, char *argv[], char **env){

    int i;
    pid_t pid;

    for(i=0; i< 10; i++){
        pid = fork();
        if(pid == 0){ //子进程
            break;
        }
    }
    if(pid ==0){
        int n = 1;
        while(n--){ //执行一次
            printf("i am child mypid=%d\n", getpid());
            sleep(2);//睡一秒结束.
        }
        return i+1;
    }
    if(pid > 0){
        sigset_t myset, oldset, ped;
        sigemptyset(&myset);
        sigaddset(&myset, SIGCHLD); //把sigchld信号阻塞
        sigprocmask(SIG_BLOCK, &myset, &oldset);
        /*while(1){ //特别注意 mac和linux平台不一样代码不通用。
            sigpending(&ped);
            printped(&ped);
            sleep(1);
        }
        */
        //在注册捕捉函数之前最好把信号阻塞住，注册完成再解除，这样就不会，子进程结束了， 父进程还没注册完。
        sleep(2); //睡2秒测试
        //注册捕捉函数        
        struct sigaction act; 
        act.sa_handler = do_child;
        act.sa_flags = 0;
        sigemptyset(&act.sa_mask); //清空信号集合
        sigaction(SIGCHLD, &act, NULL);
        sigdelset(&act.sa_mask, SIGCHLD);
        sigprocmask(SIG_SETMASK, &oldset, NULL);
        //注册捕捉函数结束
        while(1){
            printf("i am parent pid=%d\n", getpid());
            sleep(1);
        }
    }

    return 0;
}
