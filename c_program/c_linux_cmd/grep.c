/*
    功能:
        模拟grep命令
    历史记录:
        作者        时间        操作        联系方式
        卢富波      2014-05-12  创建        1164830775@qq.com

    FIXME
        暂时不实现递归
*/
#include "lib/command_comm.h"
#include "lib/str_match.h"
#include <getopt.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

static void
grep_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s FINDED_STR FILE_LIST\n", program_name);
    }

    exit(status);
}

static int
is_can_grepfile(const char *file)
{
    struct stat         stabuf;

    if(stat(file, &stabuf) != 0)
    {
        fprintf(stderr,
                "stat file [%s] error [%s]\n",
                file, strerror(errno));
        return 0;
    }

    if(S_ISREG(stabuf.st_mode))
        return 1;

    return 0;
}

static int
grep(const char *find, const char *file)
{
    char            line_buf[MAX_LINE_BUF_SIZE];
    FILE            *fp;

    if( find == NULL ||
        find[0] == 0 ||
        file == NULL ||
        file[0] == 0)
        return 1;

    if(!is_can_grepfile(file))
        return 0;

    if((fp = fopen(file, "r")) == NULL)
    {
        fprintf(stderr,
                "fopen [%s] error [%s]\n",
                file, strerror(errno));
        return 1;
    }

    while(fgets(line_buf, MAX_LINE_BUF_SIZE, fp) != NULL)
    {
        if(is_matched(find, line_buf))
            printf("%s:\t%s", file, line_buf);
    }

    if(ferror(fp))
    {
        fprintf(stderr,
                "fgets [%s] error [%s]\n",
                file, strerror(errno));
        return 1;
    }

    return 0;
}

int
main(int argc, char *argv[])
{
    int                 c;
    char                *pfind = NULL;
    int                 err;

    static struct option const long_options[] =
    {
        {"help", 0, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };
    exit_status = EXIT_SUCESS;
    program_name = argv[0];
    while((c = getopt_long(argc, argv,
                           "h", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                grep_usage_exit(EXIT_SUCESS);
                break;
            default:
                grep_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    if(argc - optind < 2)
        grep_usage_exit(EXIT_FAILURE);

    pfind = argv[optind++];
    while(optind < argc)
    {
        if((err = grep(pfind, argv[optind++])) != 0)
            exit_status = EXIT_FAILURE;
    }

    exit(exit_status);
}

