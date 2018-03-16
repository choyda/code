/*
    df.c
    功能:
        模拟df命令
    历史记录:
        作者        时间        操作        联系方式
        卢富波      2014-06-03  创建        1164830775@qq.com
*/

#include "lib/command_comm.h"
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <sys/vfs.h>
#include <errno.h>
#include <math.h>

#define SIZE1 100
#define FN "/etc/mtab"
#define SPACE ' '

static void
df_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s \n", program_name);
        printf("-h(-H)  --help  show help information\n");
    }

    exit(status);
}


static int
displayapartition(char * pt,char * pt1)
{
    struct statfs buf;
    const int block_size = 1024;
    unsigned int total;
    unsigned int used;
    unsigned int free;
    unsigned int per = 100;

    if(statfs(pt1, &buf) != 0)
    {
        fprintf(stderr,
                "statfs[%s] error[%s]\n",
                pt1, strerror(errno));
        return errno;
    }

    total = buf.f_blocks*(buf.f_bsize/block_size);
    used = (buf.f_blocks-buf.f_bfree)*(buf.f_bsize/block_size);
    free = buf.f_bavail*(buf.f_bsize/block_size);
    if(total != 0)
        per = ceil(used/(total/100));

    printf("\t%-40s%-12d%-12d%-12d%d%%\t%s\n",
           pt, total, used, free, per, pt1);

    return 0;
}

static int
df()
{
    char tmpline[SIZE1];
    FILE * fp;
    char * pt1;
    char * pt2;
    char * pt3;


    if( (fp = fopen(FN,"r")) == NULL )
    {
        fprintf(stderr,"%s \n",strerror(errno));
        exit(5);
    }

    printf("\t%-40s%-12s%-12s%-12s%-12s%-12s\n",
           "Filesystem", "1k-block", "Used", "Free", "Used_Per", "mount point");
    while( fgets(tmpline, SIZE1, fp) != NULL )
    {
        pt1=strchr(tmpline, SPACE);
        pt2=pt1+sizeof(char);
        *pt1='\0';
        pt3=strchr(pt2,SPACE);
        *pt3='\0';
        displayapartition(tmpline,pt2);
    }
    return 0;
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
                           "hHai", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                df_usage_exit(EXIT_SUCESS);
                break;
            default:
                df_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    if(argc != optind )
        df_usage_exit(EXIT_FAILURE);

    exit_status = df();

    exit(exit_status);
}
