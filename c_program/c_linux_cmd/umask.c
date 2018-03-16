/*
    ����:
        ģ��umask����
    ��ʷ��¼:
        ����        ʱ��        ����        ��ϵ��ʽ
        ¬����      2014-05-12  ����        1164830775@qq.com

    FIXME:
        ����umaskֵ���ɽ��̿��Ƶģ����ӽ��̿ɼ̳и����̵�
        (1) �����umask�����������ʾ�ɸ����̼̳ж�����umaskֵ
        (2) Ҫ�޸�umaskֵ��ֻ����sh���������޸ģ������ʱ��ûдsh����
        (3) ����Ҫдshell����ʱ����ֱ�ӵ���lib/umask.h�еĽӿ�ʵ��

*/

#include "lib/umask.h"
#include "lib/command_comm.h"
#include <getopt.h>
#include <stdlib.h>


static void
umask_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s \n", program_name);
    }

    exit(status);
}

int
main(int argc, char *argv[])
{
    mode_t          mo;
    char            tmp[8] = {0}; 
    
    if(argc != 1)
        umask_usage_exit(1);

    mo = get_mode();
    mode_to_str(get_mode(), tmp, 8);
    printf("%s\n", tmp);
    
    exit_status= 0;
    exit(exit_status);
}

