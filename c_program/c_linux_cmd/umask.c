/*
    功能:
        模拟umask命令
    历史记录:
        作者        时间        操作        联系方式
        卢富波      2014-05-12  创建        1164830775@qq.com

    FIXME:
        由于umask值是由进程控制的，由子进程可继承父进程的
        (1) 这里的umask命令仅仅能显示由父进程继承而来的umask值
        (2) 要修改umask值，只能在sh程序里面修改，这里，暂时还没写sh程序
        (3) 将来要写shell程序时，可直接调用lib/umask.h中的接口实现

*/

#include "lib/umask.h"
#include "lib/command_comm.h"
#include <getopt.h>
#include <stdlib.h>


static void
umask_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s \n", program_name);
    }

    exit(status);
}

int
main(int argc, char *argv[])
{
    mode_t          mo;
    char            tmp[8] = {0}; 
    
    if(argc != 1)
        umask_usage_exit(1);

    mo = get_mode();
    mode_to_str(get_mode(), tmp, 8);
    printf("%s\n", tmp);
    
    exit_status= 0;
    exit(exit_status);
}

