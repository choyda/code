/*
    mv.c
    ����:
        ģ��mv����

    ��ʷ��¼:
        ʱ��        ����        ����        ��ϵ��ʽ
        2014-04-26  ¬����      ����        1164830775@qq.com

    FIXME:
        �����������Ϊ����������������:cp, rm����ˣ�������Ĵ�����٣������ǵ���cp���rm����
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "lib/command_comm.h"
#include "lib/mkdir.h"
#include <getopt.h>

static void
mkdir_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s  [OPTION]DIR ...\n", program_name);
    }

    exit(status);
}

int main(int argc, char *argv[])
{
    int                 c;
    int                 is_p = 0;           /*�Ƿ�ݹ鴴��Ŀ¼                  */
    mode_t              mode;               /*����Ŀ¼��Ȩ��*/

    static struct option const long_options[] =
    {
        {"help", 0, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };

    program_name = argv[0];
    while((c = getopt_long(argc, argv,
                           "hHpP", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                mkdir_usage_exit(EXIT_SUCESS);
                break;
            case 'p':
            case 'P':
                is_p = 1;
                break;
            default:
                mkdir_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    mode = 0;
    for(c = 0; c < 9; ++c)
        mode |= 1<<c;

    if(optind < argc)
    {
        while(optind < argc)
        {
            if(is_p)
                exit_status = _mkdir_p(argv[optind++], mode);
            else
                exit_status = _mkdir(argv[optind++], mode);

            if(exit_status != 0)
                exit(exit_status);

        }
    }
    else
    {
        mkdir_usage_exit(EXIT_FAILURE);
    }

    exit_status = 0;
    exit(exit_status);
}

