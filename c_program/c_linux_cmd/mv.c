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
#include "lib/command_comm.h"
#include "lib/copy.h"
#include <getopt.h>

struct cp2mv_option     g_option;       /*�û��в�����cp�Ĳ�����һ����      */



static void
mv_exit(void)
{

}

int main(int argc, char *argv[])
{
    int                 c;
    const int           diff_optind = 2;    /*���������⣬Ҫ����SOURCE DST�ĸ���*/
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


