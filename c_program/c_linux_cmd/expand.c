/*
    功能:
        模拟expand命令  (将tab键转换为空格)
    历史记录:
        作者        时间        操作        联系方式
        卢富波      2014-06-16  创建        1164830775@qq.com
*/
#include "lib/command_comm.h"
#include "lib/err_msg.h"
#include "lib/file_isexist.h"
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>


static void
expand_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s       FILE\n", program_name);
        printf("-h(-H)  --help  Show help information\n");
    }

    exit(status);
}

static void
change(char *buf, char src, char dst)
{
    size_t              i = 0;

    while(1)
    {
        if(buf[i] == 0)
            break;
        if(buf[i] == src)
            buf[i] = dst;

        ++i;
    }
}

static int
expand(const char *file)
{
    FILE                *fp;
    char                buf[MAX_LINE_BUF_SIZE];
    
    if(!file_isreg(file))
        pr_msg2exit(EXIT_FAILURE, "file [%s] is not simple file or had not existed\n", file);

    if((fp = fopen(file, "r")) == NULL)
        pr_msg2exit(EXIT_FAILURE, "open file [%s] error[%s]\n", file, strerror(errno));

    while(fgets(buf, MAX_LINE_BUF_SIZE, fp) != NULL){
        change(buf, '\t', ' ');
        fputs(buf, stdout);
    }

    if(ferror(fp))
       pr_msg2exit(EXIT_FAILURE, "fgets [%s] error\n", file); 
    if(ferror(fp))
       pr_msg2exit(EXIT_FAILURE, "fputs [%s] error\n", "stdout");

    fclose(fp);

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
                expand_usage_exit(EXIT_SUCESS);
                break;
            default:
                expand_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    if(argc - optind != 1)
        expand_usage_exit(EXIT_FAILURE);

    if(expand(argv[optind]) != 0)
        exit_status = EXIT_FAILURE;

    exit(exit_status);
}

