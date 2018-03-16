/*
    功能:
        模拟rm命令
    历史记录:
        作者        时间        操作        联系方式
        卢富波      2014-04-27  创建        1164830775@qq.com

*/

#include "lib/remove.h"
#include "lib/command_comm.h"
#include <getopt.h>
#include <stdlib.h>


static void
rm_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s  FILE[DIRECOTORY] OPTION\n", program_name);
        printf("-h  --help(--h) show the help infomation\n");
        printf("-f[-F]  --force(--f) force to remove without prompt\n");
        printf("-r[-R]  --recure(--r) remove directories and their contents recursivelyn\n");
    }

    exit(status);
}

int
main(int argc, char *argv[])
{
    int                 c;
    struct  rm_option   rmoption;

    memset(&rmoption, 0, sizeof(rmoption));
    
    exit_status = 0;
    static struct option const long_options[] =
    {
        {"help", 0, NULL, 'h'},
        {"recure", 0, NULL, 'r'},
        {"fource", 0, NULL, 'f'},
        {NULL, 0, NULL, 0}
    };

    program_name = argv[0];
    while((c = getopt_long(argc, argv,
                           "hHrRfF", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                rm_usage_exit(EXIT_SUCESS);
                break;
            case 'r':
            case 'R':
                rmoption.r = 1;
                break;
            case 'f':
            case 'F':
                rmoption.f = 1;
                break;
            default:
                rm_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    if(optind == argc)
       rm_usage_exit(1); 

    while(optind < argc)
        exit_status |= rm(argv[optind++], &rmoption);

    exit(exit_status);
}

