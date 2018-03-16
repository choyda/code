/*
    模拟pwd命令

    历史记录:
    日期        作者        描述        联系方式
    2014.05.15  卢富波      首次创建    164830775@qq.com
*/

#include "lib/command_comm.h"
#include "lib/pw.h"
#include <stdlib.h>
#include <getopt.h>

char                *pwd = NULL;

static void
pwd_usage_exit(int status)
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

static void
pwd_exit(void)
{
    if(pwd != NULL)
    {
        free(pwd);
        pwd = NULL;
    }
}

int
main(int argc, char *argv[])
{
    int                 c;

    static struct option long_options[] =
    {
        {"help", 0, 0, 'h'},
        {0, 0, 0, 0}
    };

    atexit(pwd_exit);
    program_name = argv[0];
    /*短参数定义-*/
    static const char short_options[] = "hH";
    while((c = getopt_long(argc, argv,
                           short_options, long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                pwd_usage_exit(EXIT_SUCESS);
                break;
            default:
                pwd_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    if(argc != optind)
        pwd_usage_exit(EXIT_FAILURE);

    if((pwd = get_pwd()) != NULL)
        printf("%s\n", pwd);
    else
        exit(EXIT_FAILURE);

    exit_status = EXIT_SUCCESS;
    exit(exit_status);
}
