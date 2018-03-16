#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
	pid_t pid;
	int fd[2];

	pipe(fd); //注意 pipe必须要放在fork前面，fork创建子进程
			  //共享父进程资源，不先创建管道，子进程中没有共享的管道，
			  //子进程不能用管道跟父进程进行通信
	pid = fork();

	if (pid == 0) {  //child
		close(fd[1]);	                //子进程从管道中读数据，关闭写端
		dup2(fd[0], STDIN_FILENO);		//让wc从管道中读取数据
		execlp("wc", "wc", "-l", NULL);	//wc命令默认从标准读入取数据

	} else {

		close(fd[0]);	//父进程向管道中写数据，关闭读端
		dup2(fd[1], STDOUT_FILENO);		//将ls的结果写入管道中
		execlp("ls", "ls", NULL);		//ls输出结果默认对应屏幕
	}

	return 0;
}
