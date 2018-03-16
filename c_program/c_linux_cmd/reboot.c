/*
    reboot.c
    功能:
        模拟reboot命令

    历史记录:
        时间        作者        操作        联系方式
        2014-05-16  卢富波      创建        1164830775@qq.com
*/

#include "lib/command_comm.h"
#include "lib/is_digit.h"
#include <unistd.h>
#include <sys/reboot.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>


static void
reboot_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s  NUM(0-6)\n", program_name);
    }

    exit(status);
}


int
main(int argc, char *argv[])
{
    program_name = argv[0];
    exit_status = 0;

    if(argc != 2)
        reboot_usage_exit(EXIT_FAILURE);

    if(is_digit(argv[1],strlen(argv[1])) == 0)
        reboot_usage_exit(EXIT_FAILURE);

    if(reboot(atoi(argv[1])) != 0)
    {
        fprintf(stderr,
                "[%s] error [%s]\n",
                program_name, strerror(errno));
        exit_status = errno;
    }

    exit(exit_status);
}
