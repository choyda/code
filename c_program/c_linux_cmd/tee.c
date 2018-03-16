/*
    tee.c
    功能:
        模拟tee命令
    历史记录:
        作者        时间        操作        联系方式
        卢富波      2014-06-11  创建        1164830775@qq.com
*/

#include "lib/command_comm.h"
#include "lib/err_msg.h"
#include "lib/file_isexist.h"
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <sys/vfs.h>
#include <errno.h>
#include <math.h>




static void
tee_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s FILE\n", program_name);
    }

    exit(status);
}

static void
setnobuf(FILE *fp)
{
    setvbuf(fp, NULL, _IONBF, 0);
}

static int
tee(const char *file)
{
    char         buffer[MAX_LINE_BUF_SIZE];
    FILE         *fp = fopen(file, "wb");
    int          bytes_read;

    if(stdin == NULL)
        return 1;

    if(fp == NULL)
    {
        pr_msg("open [%s] error[%s]\n", file, strerror(errno));

        return 1;
    }
    setnobuf(stdout);
    setnobuf(fp);

    while (1)
    {
        bytes_read = read (0, buffer, MAX_LINE_BUF_SIZE);
        if (bytes_read < 0 && errno == EINTR)
            continue;
        if (bytes_read <= 0)
            break;

        fwrite(buffer, bytes_read, 1, stdout);
        fwrite(buffer, bytes_read, 1, fp);
    }

    if(bytes_read == -1)
    {
        pr_msg("read from stdin error\n");
        fclose(fp);
        return 1;
    }
    if(ferror(stdout))
    {
        pr_msg("write stdout error\n");
        fclose(fp);
        return 1;
    }
    if(ferror(fp))
    {
        pr_msg("fwrite [%s] error\n", file);
        fclose(fp);
        return 1;
    }

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
    exit_status = 0;
    program_name = argv[0];
    while((c = getopt_long(argc, argv,
                           "h", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                tee_usage_exit(EXIT_SUCESS);
                break;
            default:
                tee_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    if(argc - optind != 1)
        tee_usage_exit(EXIT_FAILURE);

    if(tee(argv[optind]) != 0)
        exit_status = EXIT_FAILURE;

    exit(exit_status);
}


