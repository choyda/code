/*
    mv.c
    功能:
        模拟mv命令

    历史记录:
        时间        作者        操作        联系方式
        2014-04-26  卢富波      创建        1164830775@qq.com

    FIXME:
        该命令我理解为，是两个命令的组合:cp, rm。因此，此命令的代码很少，基本是调用cp命令及rm命令
*/

#include <stdio.h>
#include "lib/command_comm.h"
#include "lib/copy.h"
#include <getopt.h>

struct cp2mv_option     g_option;       /*用户有参数与cp的参数是一样的      */



static void
mv_exit(void)
{

}

int main(int argc, char *argv[])
{
    int                 c;
    const int           diff_optind = 2;    /*除开参数外，要传入SOURCE DST的个数*/
    char                *psrc, *pdst;

    static struct option const long_options[] =
    {
        {"help", 0, NULL, 'h'},
        {"recursive", 0, NULL, 'r'},
        {"link", 0, NULL, 'l'},
        {"fource", 0, NULL, 'f'},
        {NULL, 0, NULL, 0}
    };

    program_name = argv[0];
    atexit(mv_exit);

    memset(&g_option, 0, sizeof(g_option));
    while((c = getopt_long(argc, argv,
                           "hHrRlLfF", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                cp2mv_usage_exit(EXIT_SUCESS);
                break;
            case 'r':
            case 'R':
                g_option.r = 1;
                break;
            case 'l':
            case 'L':
                g_option.l = 1;
                break;
            case 'f':
            case 'F':
                g_option.f = 1;
                break;
            default:
                cp2mv_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    if(argc - optind != diff_optind)
        cp2mv_usage_exit(EXIT_FAILURE);

    psrc = argv[optind++];
    pdst = argv[optind++];
    assert(psrc != NULL && psrc[0] != 0);
    assert(pdst != NULL && pdst[0] != 0);

    cp(psrc, pdst, &g_option, 1);

    exit_status = EXIT_SUCCESS;
    exit(exit_status);
}


