/*
    rmdir.c
    ����:
        ģ��rmdir����
    ��ʷ��¼:
        ����        ʱ��        ����        ��ϵ��ʽ
        ¬����      2014-05-18  ����        1164830775@qq.com
*/

#include "lib/rmdir.h"
#include "lib/command_comm.h"
#include <getopt.h>
#include <stdlib.h>


static void
rmdir_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s  DIR1 [DIR2 ...]\n", program_name);
        printf("-h  --help(--h) show the help infomation\n");
    }

    exit(status);
}


int
main(int argc, char *argv[])
{
    int                     c;
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
                rmdir_usage_exit(EXIT_SUCESS);
                break;
            default:
                rmdir_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    if(optind == argc)
        rmdir_usage_exit(EXIT_FAILURE);

    while(optind < argc)
        exit_status = _rmdir(argv[optind++]);

    exit(exit_status);
}

