/*
    lsattr.c
    功能:
        模拟lsattr命令:显示文件隐藏属性(暂时只考虑a i属性)
    历史记录:
        作者        时间        操作        联系方式
        卢富波      2014-05-16  创建        1164830775@qq.com
*/

#include "lib/command_comm.h"
#include "lib/fileflag.h"
#include "lib/file_isexist.h"
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>


static void
lsattr_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s  FILE1 FILE2 ...\n", program_name);
    }

    exit(status);
}

static void
judge_ttr(char *att_msg, unsigned int attr)
{
#define I_OFFSET   4
#define A_OFFSET   5
    if(attr & (1 << I_OFFSET))
        att_msg[I_OFFSET] = 'i';
    
    if(attr & (1 << A_OFFSET))
        att_msg[A_OFFSET] = 'a';
#undef A_OFFSET
#undef I_OFFSET
}

static int 
pr_file_attr(const char *file)
{
    unsigned int        attr;
    int                 err;
    char                att_msg[13];

    if(file_isexist(file) == 0)
    {
        fprintf(stderr,
                "file [%s] does not existed\n",
                file);
        return 1;
    }

    memset(att_msg, '-', sizeof(att_msg));
    if((err = _fgetflags(file, &attr)) != 0)
        return err;

    judge_ttr(att_msg, attr);

    printf("%s  %s\n", att_msg, file);

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
                           "hH", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                lsattr_usage_exit(EXIT_SUCESS);
                break;
            default:
                lsattr_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    if(argc == optind )
        lsattr_usage_exit(EXIT_FAILURE);

    while(optind < argc)
    {
        exit_status |= pr_file_attr(argv[optind++]);
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

