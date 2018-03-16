#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
//wait 循环回收子进程
int main(int argc, char * argv, char * env){
    pid_t pid;
    int w_pid, i, status;
    for(i=0; i<5; i++){
        pid = fork();
        if(pid == -1){
            perror("fork error:");
        }
        if(pid == 0){
            sleep(i);
            printf("im child%d\n", getpid());
            break;
        }
    }

    if(i == 5){
        sleep(8);

        /*(waitpid异步回收全部子进程)
        while(i){
            w_pid = waitpid(-1, NULL, WNOHANG);
            if(w_pid > 0){
                i--;
            }
        }
        */
        // waitpid(0, NULL, 0);  waitpid按组回收
        //wait循环回收
        while(w_pid=wait(NULL)){
            if(w_pid == -1) break;
            sleep(1);
            printf("%d\n",w_pid);
        }

    }
    return 111;

}
