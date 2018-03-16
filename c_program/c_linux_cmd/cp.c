/*
    ģ��cp������Ҫ����
    ����:
        ���cp�������Ҫ����
        (1) �����ļ��Ĵ���
        (2) ���ļ�������ʾ����
        (3) �Ƿ�����ݹ鸴��
        (4) ����ǿ�Ƹ���
    �ݲ����ǵ�:
        (1) �Կն��ļ��Ĵ���
        (2) ���ļ�Ȩ�޵Ĵ���
        (3) ���ļ��ǲ�������ʱ�����ƵĴ���
        (4) �Բ�ͬ�ļ�ϵͳ֮��ĸ��ƴ���
        (5) ��ͨ����Ĵ���
*/
#include "lib/copy.h"
#include "lib/command_comm.h"
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>



struct cp2mv_option        g_optin;    /* �û�����˵��      */



static void
cp_exit(void)
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



