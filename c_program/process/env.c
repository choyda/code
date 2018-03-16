/*************************************************************************
 > File Name: env.c
 > Author: 
 > Mail: 
 > Created Time: 三 11/29 15:00:49 2017
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char **environ;
int main(int argc, char **argv, char **env){

    char * env_name;
    int ex;
    putenv("HAHA=haha");
    setenv("HEHE","hehe", 1);
    
    //unsetenv("CAI");
    //ex = execl("./exec", NULL); //只有在当前脚本执行才能显示，上面定义的环境变量， 
                                //单独执行exec是没有定义的，这个是父子进程的关系
                                //execl调用相当于fork一个子进程，所以就会有 
    //printf("%d", ex);
    //env_name = getenv("PATH");
    //printf("%s",env_name);

    int i;
    for(i=0; environ[i]; i++){
        printf("%s\n", environ[i]);
    }
    return 0;
}
