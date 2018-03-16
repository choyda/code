/*
    chgrp.c
    功能:
        模拟chgrp命令:创建空文件或者修改文件时间
    历史记录:
        作者        时间        操作        联系方式
        卢富波      2014-05-16  创建        1164830775@qq.com
*/

#include "lib/command_comm.h"
#include "lib/chown.h"
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

void
chgrp_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s  USERS  FILE1 FILE2 ...\n", program_name);
        printf("-h  --help(--h) show the help infomation\n");
    }

    exit(status);
}

int
main(int argc, char *argv[])
{
    const int           off_op = 2;     /*  至少带的参数个数    */
    int                 c;
    char                *user = NULL;

    static struct option const long_options[] =
    {
        {"help", 0, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };

    exit_status = 0;
    program_name = argv[0];

    while((c = getopt_long(argc, argv,
                           "hH", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                chgrp_usage_exit(EXIT_SUCESS);
                break;
            default:
                chgrp_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    if(argc - optind < off_op)
        chgrp_usage_exit(EXIT_FAILURE);

    assert(argv[optind] != NULL);
    user = argv[optind++];

    while(optind < argc)
    {
        exit_status |= _chgrp(argv[optind++], user);
    }

    if(exit_status)
    {
        if(exit_status > 0)
            fprintf(stderr,
                    "[%s] error with errno msg[%s]\n",
                    program_name, strerror(exit_status));
        else
            fprintf(stderr,
                    "[%s] error with errno no[%d]\n",
                    program_name, exit_status);
    }

    exit(exit_status);
}
