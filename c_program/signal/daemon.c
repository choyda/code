/*************************************************************************
 > File Name: daemon.c
 > Author: 
 > Mail: 
 > Created Time: 二 12/26 17:33:42 2017
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

int main(){
	//当前时间
    time_t timep;  
    char s[30];  
    
    pid_t pid, sid;
    int fd, wfd;

    //1 创建子进程
    pid = fork();
    if(pid > 0){
        return 0; //如果是父进程就退出
    }

    //2 创建会话
    sid = setsid();

    //3 设置目录
    chdir("~/code/deamon/");

    //4 umask设置权限
    umask(0002);

    //5 关闭文件描述符
    close(STDIN_FILENO); // 关闭0 标准输入的文件描述符。
    fd = open("/dev/null", O_RDWR); //打开一个文件，因为0关闭了，所以这个文件描述符是0
    dup2(0, STDOUT_FILENO);
    dup2(0, STDERR_FILENO);

    //6 主控逻辑
    while(1){
		//每隔1秒写一下时间
        time(&timep);  
        strcpy(s,ctime(&timep));  
		wfd = open("time.txt", O_RDWR|O_CREAT|O_TRUNC, 0664);
		write(wfd, s, strlen(s));	
		sleep(1);
    }

    //7 关闭逻辑， 接受信号进行资源回收等


    return 0;
}
