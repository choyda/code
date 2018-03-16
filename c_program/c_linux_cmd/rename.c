/*
    rename.c
    ����:
        ģ��rename����

    ��ʷ��¼:
        ʱ��        ����        ����        ��ϵ��ʽ
        2014-05-09  ¬����      ����        1164830775@qq.com
*/
#include "lib/command_comm.h"
#include "lib/rename.h"
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>

void
rename_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s  oldname newname\n", program_name);
    }

    exit(status);
}

int
main(int argc, char *argv[])
{
    int                 c;
    const int           diff_optind = 2;    /*���������⣬Ҫ����Ĳ�������*/
    char                *pold, *pnew;


    static struct option const long_options[] =
    {
        {"help", 0, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };

    program_name = argv[0];
    while((c = getopt_long(argc, argv,
                           "hH", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                rename_usage_exit(EXIT_SUCESS);
                break;
            default:
                rename_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    if(argc - optind != diff_optind)
        rename_usage_exit(EXIT_FAILURE);

    pold = argv[optind++];
    pnew = argv[optind++];

    exit_status = _rename(pold, pnew);

    exit(exit_status);
}

