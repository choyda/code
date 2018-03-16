/*
    find.c
    功能:
        模拟find命令 仅实现匹配符格式(-name )
    历史记录:
        作者        时间        操作        联系方式
        卢富波      2014-06-02  创建        1164830775@qq.com
*/

#include "lib/command_comm.h"
#include "lib/find_file.h"
#include "lib/file_isexist.h"
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>


static void
find_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s  DIRECTORY [FILENAME or MATCH_NAME]\n", program_name);
        printf("-h(-H)  --help      show help information\n");
        printf("\nNOTE              you must run as root\n");
    }

    exit(status);
}

static int
find (int argc, char *argv[])
{

    if(argc == 1)
        return scan_dir(argv[0], ".*", find_matched, find_ope, 0);

    return scan_dir(argv[0], argv[1], find_matched, find_ope, 0);
}

int
main(int argc, char *argv[])
{
    int                 c;

    static struct option const long_options[] =
    {
        {"help", 0, NULL, 'h'},
        {"name", 1, NULL, 'n'},
        {NULL, 0, NULL, 0}
    };
    exit_status = 0;
    program_name = argv[0];
    while((c = getopt_long(argc, argv,
                           "HN", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                find_usage_exit(EXIT_SUCESS);
                break;
            default:
                find_usage_exit(EXIT_FAILURE);
                break;
        }
    }



    argc -= optind;
    argv += optind;

    if(argc - optind > 2)
        find_usage_exit(EXIT_FAILURE);

    exit_status = find(argc, argv);

    exit(exit_status);
}


