/*
    ln.c
    功能:
        模拟ln命令
    历史记录:
        作者        时间        操作        联系方式
        卢富波      2014-06-03  创建        1164830775@qq.com
*/

#include "lib/command_comm.h"
#include "lib/link.h"
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>

static void
ln_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s  SORCEFILE DSTFILE\n", program_name);
        printf("-s              create soft link instead of hard link\n");
        printf("-h(-H)  --help  show help information\n");
    }

    exit(status);
}


int
main(int argc, char *argv[])
{
    int                 c;
    int                 is_softlink = 0;

    static struct option const long_options[] =
    {
        {"help", 0, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };
    exit_status = 0;
    program_name = argv[0];
    while((c = getopt_long(argc, argv,
                           "Hs", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                ln_usage_exit(EXIT_SUCESS);
                break;
            case 's':
                is_softlink = 1;
                break;
            default:
                ln_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    if(argc - optind != 2)
        ln_usage_exit(EXIT_FAILURE);

    if(is_softlink)
        exit_status = create_softlink(argv[optind], argv[optind+1]);
    else
        exit_status = create_hardlink(argv[optind], argv[optind+1]);

    exit(exit_status);
}


