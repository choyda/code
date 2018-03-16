/*
    chmod.c
    功能:
        模拟chmod命令:仅支持带绝对属性的
    历史记录:
        作者        时间        操作        联系方式
        卢富波      2014-05-15  创建        1164830775@qq.com
*/

#include "lib/command_comm.h"
#include "lib/chmod.h"
#include "lib/umask.h"
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>

static void
chmod_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s  FILE1 FILE2 ... [OPTION] MODE, the MODE must last arg\n", program_name);
        printf("-h  --help(--h) show the help infomation\n");
        printf("-r              recursion change mode\n");
    }

    exit(status);
}

static int
fill_mode(mode_t *mode, char *arg)
{
    int                 err;

    if((err = str_to_mode(mode, arg, strlen(arg))) != 0)
        return err;

    return 0;
}

int
main(int argc, char *argv[])
{
    int                     c;
    int                     is_r = 0;
    mode_t                  mode;

    static struct option const long_options[] =
    {
        {"help", 0, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };

    program_name = argv[0];
    exit_status = 0;
    while((c = getopt_long(argc, argv,
                           "hrR", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                chmod_usage_exit(EXIT_SUCESS);
                break;
            case 'r':
            case 'R':
                is_r = 1;
                break;
            default:
                chmod_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    if(argc - optind < 2)   /*  到少带两个命令行选项进来  */
        chmod_usage_exit(EXIT_FAILURE);

    if((exit_status = fill_mode(&mode, argv[argc-1])) != 0)
        exit(exit_status);

    while(optind < argc - 1)    /*  最后位是mode_t类型 */
        exit_status |= _chmod(argv[optind++], mode, is_r);

    if(exit_status != 0)
        fprintf(stderr,
                "errno[%d] errmsg[%s]\n",
                exit_status, strerror(exit_status));

    exit(exit_status);
}

