/*
    who.c
    功能:
        模拟who命令
    历史记录:
        作者        时间        操作        联系方式
        卢富波      2014-05-29  创建        1164830775@qq.com
*/

#include "lib/command_comm.h"
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <utmp.h>
#include <time.h>
#include <string.h>

static void
who_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s \n", program_name);
        printf("-h(-H)  --help  show help information\n");
    }

    exit(status);
}


static int
pr_who()
{
    struct utmp         *u;
    char                timebuf[128];

    while((u = getutent()))
    {
        /*  去除一些不需要显示的项  */
        if(u->ut_type != USER_PROCESS)
            continue;
        ctime_r(&(u->ut_tv.tv_sec), timebuf);
        if(timebuf[0] != 0)
            timebuf[strlen(timebuf)-1] = 0; /*去掉'\n'*/
        printf("%-12s%-12s%-20.20s  (%s)\n",
               u->ut_user,
               u->ut_line,
               timebuf,
               u->ut_host);
    }

    endutent();
    return 0;
}

int
main(int argc, char *argv[])
{
    int                 c;

    static struct option const long_options[] =
    {
        {"help", 0, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };
    exit_status = 0;
    program_name = argv[0];
    while((c = getopt_long(argc, argv,
                           "hHai", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                who_usage_exit(EXIT_SUCESS);
                break;
            default:
                who_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    if(argc != optind )
        who_usage_exit(EXIT_FAILURE);

    exit_status = pr_who();

    exit(exit_status);
}

