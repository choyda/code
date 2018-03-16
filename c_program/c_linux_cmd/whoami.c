/*
    whoami.c
    功能:
        模拟whoami命令
    历史记录:
        作者        时间        操作        联系方式
        卢富波      2014-05-29  创建        1164830775@qq.com
*/

#include "lib/command_comm.h"
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <pwd.h>


static void
whoami_usage_exit(int status)
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
pr_whoami()
{
    struct passwd   *pass = getpwuid(geteuid());

    if(pass == NULL)
        return errno;

    printf("%s\n", pass->pw_name);
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
                whoami_usage_exit(EXIT_SUCESS);
                break;
            default:
                whoami_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    if(argc != optind )
        whoami_usage_exit(EXIT_FAILURE);

    exit_status = pr_whoami();

    exit(exit_status);
}

