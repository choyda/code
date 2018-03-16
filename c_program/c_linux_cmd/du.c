/*
    du.c
    ����:
        ģ��du����:����Ŀ¼�ǰ�Ŀ¼����������ļ��ۼӵó����ֽ���
    ��ʷ��¼:
        ����        ʱ��        ����        ��ϵ��ʽ
        ¬����      2014-06-03  ����        1164830775@qq.com
*/

#include "lib/command_comm.h"
#include "lib/get_filesize.h"
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>

static void
du_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s  FILE list (DIRECTORY list)\n", program_name);
        printf("-h(-H)  --help  show help information\n");
    }

    exit(status);
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
                           "H", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                du_usage_exit(EXIT_SUCESS);
                break;
            default:
                du_usage_exit(EXIT_FAILURE);
                break;
        }
    }
    
    if(argc == optind )
        du_usage_exit(EXIT_FAILURE);

    while(optind < argc)
        exit_status |= get_dirsize(argv[optind++]);

    exit(exit_status);
}


