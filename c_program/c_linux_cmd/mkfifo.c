/*
    mkfifo.c
    功能:
        模拟mkfifo命令
    历史记录:
        作者        时间        操作        联系方式
        卢富波      2014-06-03  创建        1164830775@qq.com
*/

#include "lib/command_comm.h"
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>


static void
mkfifo_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s  FILENAME\n", program_name);
        printf("-h(-H)  --help  show help information\n");
    }

    exit(status);
}

int
main(int argc, char *argv[])
{
    int                 c, err;
    mode_t              newmode = (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    static struct option const long_options[] =
    {
        {"help", 0, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };
    exit_status = 0;
    program_name = argv[0];
    while((c = getopt_long(argc, argv,
                           "H", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                mkfifo_usage_exit(EXIT_SUCESS);
                break;
            default:
                mkfifo_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    if(argc == optind )
        mkfifo_usage_exit(EXIT_FAILURE);

    while(optind < argc)
    {
        err = mkfifo(argv[optind++], newmode);
        if(err != 0)
        {
            fprintf(stderr,
                    "mkfifo [%s] error[%s]\n",
                    argv[optind-1], strerror(errno));
            exit_status = 1;
        }
    }

    exit(exit_status);
}



