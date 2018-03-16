/*
    sed.c
    功能:
        模拟sed命令  (将字符串替换，从标准输入读入)
    历史记录:
        作者        时间        操作        联系方式
        卢富波      2014-06-12  创建        1164830775@qq.com
*/
#include "lib/command_comm.h"
#include "lib/str_match.h"
#include "lib/err_msg.h"
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>


char                    *g_del_str = NULL;

static void
sed_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {   
        printf("Usage: %s       SRC1 SRC2\n", program_name);
        printf("Replace the SRC1 with SRC2\n");
        printf("only read from stdin\n");
        printf("-h(-H)  --help  Show help information\n");
    }

    exit(status);
}

static void 
sed_oneline ( char *linebuf, char *src, char *dst)
{
    struct finded_str   re[1024];
    int                 rc,i;
    unsigned int        last = 0;

    rc = find_str(src, linebuf,re, 1024);
    if(rc <= 0){
        write(STDOUT_FILENO, linebuf, strlen(linebuf));
        return;
    }
    
    for(i = 0; i < rc; ++i){
        write(STDOUT_FILENO, linebuf+last, re[i].begin-last);
        write(STDOUT_FILENO, dst, strlen(dst));
        last = re[i].end;
    }
    write(STDOUT_FILENO, linebuf+last,strlen(linebuf)-last);
    printf("\n");
}

static int
sed(char *src, char *dst)
{
    char                buf[MAX_LINE_BUF_SIZE];

    if(src == NULL || src[0] == 0 ||
        dst == NULL || dst[0] == 0){
        pr_msg("had some null string, ERRORS");
        return 1;
    }

    while(fgets(buf, MAX_LINE_BUF_SIZE, stdin) != NULL)
        sed_oneline(buf, src, dst);

    if(ferror(stdin)){
        pr_msg("fgets stdin error [%s]\n", strerror(errno));
        return 1;
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
    exit_status = EXIT_SUCESS;
    program_name = argv[0];
    while((c = getopt_long(argc, argv,
                           "hH", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                sed_usage_exit(EXIT_SUCCESS);
                break;
            default:
                sed_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    if(argc - optind != 2)
        sed_usage_exit(EXIT_FAILURE);

    if(sed(argv[optind], argv[optind+1]))
        exit_status = EXIT_FAILURE;

    exit(exit_status);
}



