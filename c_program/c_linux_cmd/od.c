/*
    od.c
    功能:
        模拟od命令:仅仅实现把文件以字符形式打印出来
    历史记录:
        作者        时间        操作        联系方式
        卢富波      2014-05-29  创建        1164830775@qq.com
*/
#include "lib/command_comm.h"
#include "lib/file_isexist.h"
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <getopt.h>

#define OD_ONE_LINE_CHAR        16

/* Names for some non-printing characters.  */
static const char *const charname[33] =
{
    "nul", "soh", "stx", "etx", "eot", "enq", "ack", "bel",
    "bs", "ht", "nl", "vt", "ff", "cr", "so", "si",
    "dle", "dc1", "dc2", "dc3", "dc4", "nak", "syn", "etb",
    "can", "em", "sub", "esc", "fs", "gs", "rs", "us",
    "sp"
};

static void
od_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s  FILE \n", program_name);
        printf("-h  --help(--h) show the help infomation\n");
    }

    exit(status);
}

static void
od_one_char(unsigned int c)
{
    unsigned int masked_c = (0x7f & c);
    const char *s;
    char buf[5];

    if (masked_c == 127)
        s = "del";
    else if (masked_c <= 040)
        s = charname[masked_c];
    else
    {
        sprintf (buf, "  %c", masked_c);
        s = buf;
    }

    printf (" %3s", s);
}

static int
od(int fd_no)
{
    char                    c;
    int                     n;
    static unsigned int     haddealed = 0;

    while((n = read(fd_no, &c, 1)) > 0)
    {
        if(haddealed % OD_ONE_LINE_CHAR == 0)
        {
            if(haddealed != 0)
                printf("\n");
            printf("%.7o", haddealed);
        }
        od_one_char(c);
        ++haddealed;
    }

    if(n < 0)
        return errno;

    printf("\n");
    printf("%.7o\n", haddealed);

    return 0;
}

static int
od_afile(char *file)
{
    int             fd;

    if(file_isreg(file) == 0)
    {
        fprintf(stderr,
                "file[%s] is not a simple file or not existed\n",
                file);

        exit(EXIT_FAILURE);
    }

    if((fd = open(file, O_RDONLY)) == -1)
        return errno;

    return od(fd);
}

int
main(int argc, char *argv[])
{
    int                     c;
    const int               off_param = 1;
    static struct option const long_options[] =
    {
        {"help", 0, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };
    program_name = argv[0];
    exit_status = 0;
    while((c = getopt_long(argc, argv,
                           "H", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                od_usage_exit(EXIT_SUCESS);
                break;
            default:
                od_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    if(argc - optind != off_param)
        od_usage_exit(EXIT_FAILURE);

    exit_status = od_afile(argv[optind]);
    if(exit_status > 0)
        fprintf(stderr,
                "od[%s]errno[%d] errmsg[%s]\n",
                argv[optind], exit_status, strerror(exit_status));

    exit(exit_status);
}

