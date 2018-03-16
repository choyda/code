/*
    which.c
    功能:
        模拟which命令
    历史记录:
        作者        时间        操作        联系方式
        卢富波      2014-06-02  创建        1164830775@qq.com
*/

#include "lib/command_comm.h"
#include "lib/find_file.h"
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>


static void
which_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s  COMMAND1 COMMAND2 ...\n", program_name);
        printf("-h(-H)  --help  show help information\n");
    }

    exit(status);
}

static int
which(int argc, char *argv[])
{
    const char *path_list = getenv("PATH");
    const char *path_sep = ":";
    char        *tmp_path = NULL;
    char        *token = NULL;
    size_t      path_len = strlen(path_list);
    int         rc = 0, i;


    if(path_len == 0)
        return 1;

    if((tmp_path = (char *)malloc(path_len+1)) == NULL)
        return 2;

    for(i = 0; i < argc; ++i)
    {
        strncpy(tmp_path, path_list, path_len+1);
        token = strtok(tmp_path, path_sep);
        while(token != NULL)
        {
            if(scan_dir(token, argv[i], which_matched, which_ope, 1) != 0)
                break;
            token = strtok(NULL, path_sep);
        }
    }

    return rc;
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
                           "h", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                which_usage_exit(EXIT_SUCESS);
                break;
            default:
                which_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    if(optind == argc)
        which_usage_exit(EXIT_FAILURE);

    argc -= optind;
    argv += optind;

    exit_status = which(argc, argv);

    exit(exit_status);
}


