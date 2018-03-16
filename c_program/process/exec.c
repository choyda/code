/*************************************************************************
 > File Name: exec.c
 > Author: 
 > Mail: 
 > Created Time: 四 11/30 09:53:22 2017
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main(void){
    int i;
    pid_t pid;
    for(i=0;i<2;i++){
        pid = fork();
        if(pid == 0){ 
            //子进程, exec执行就跳出，如果执行正常就不会在返回调用处，
            //新进程test的进程id不变还是子进程的id，其他的.text .data段会替换掉。
            //``execl("ls", "ls", "-l", "-a", NULL);
            char *argvs[3];
            argvs[0] = "ls";
            argvs[1] = "-l";
            argvs[2] = "-a";
            argvs[3] = NULL;
            execve("./env", argvs, argvs);
        }
    }
    return 0; 
}
