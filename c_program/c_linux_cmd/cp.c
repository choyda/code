/*
    模拟cp命令主要功能
    需求:
        完成cp命令的主要功能
        (1) 链接文件的处理
        (2) 当文件存在提示覆盖
        (3) 是否允许递归复制
        (4) 允许强制覆盖
    暂不考虑的:
        (1) 对空洞文件的处理
        (2) 对文件权限的处理
        (3) 对文件是不是最新时不复制的处理
        (4) 对不同文件系统之间的复制处理
        (5) 对通配符的处理
*/
#include "lib/copy.h"
#include "lib/command_comm.h"
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>



struct cp2mv_option        g_optin;    /* 用户参数说明      */



static void
cp_exit(void)
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
    atexit(cp_exit);

    memset(&g_optin, 0, sizeof(g_optin));
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
                g_optin.r = 1;
                break;
            case 'l':
            case 'L':
                g_optin.l = 1;
                break;
            case 'f':
            case 'F':
                g_optin.f = 1;
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

    cp(psrc, pdst, &g_optin, 0);

    exit_status = EXIT_SUCCESS;
    exit(exit_status);
}



