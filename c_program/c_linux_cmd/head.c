/*
    描述:
        模拟head命令
    需求:
        1   如果不带参数显示前面10行
        2   -n=XX 只显示前XX行
        3   -c=XX 只显示前XX字节
            如果同时指定了-c -n 则按-c处理
*/

#include "lib/command_comm.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <assert.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

struct head_option
{
    int             n;      /* 显示前n行        */
    int             c;      /* 显示前c字节      */
};

struct head_option      g_option;

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

static void
init_option()
{
    g_option.n = 10;         /* 默认是显示前10行     */
    g_option.c = 0;         /* 默认显示的字节数     */
}

static void
head_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help -h' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s [OPTION] FILE\n ", program_name);
        printf("OPTION\n");
        printf("-h -H  --help           Help infomation\n");
        printf("-c, --bytes=N           Show first N bytes\n");
        printf("-n, --lines=N           Show first N lines\n");
    }

    exit(status);
}

static void
head_exit(void)
{

}

/*判断字符串全部是数字*/
static int
is_digit(char *ps)
{
    size_t      i, len = strlen(ps);

    if(len == 0)
        return 0;

    for(i = 0; i < len; ++i)
    {
        if(i == 0 && ps[0] == '-')
            continue;
        if(!isdigit(ps[i]))
            return 0;
    }

    return 1;
}

static void
head_c(const char *file)
{
    assert(g_option.c != 0);
    char                buf[MAX_LINE_BUF_SIZE];
    int                 need_read_size = g_option.c;
    int                 n;
    int                 f;

    if((f = open(file, O_RDONLY)) < 0)
    {
        fprintf(stderr, "open file '%s' error: %s\n", file, strerror(errno));
        close(f);
        exit(1);
    }

    while((need_read_size > 0) &&
          ((n = read(f, buf, MIN(MAX_LINE_BUF_SIZE, need_read_size))) > 0))
    {
        if(write(STDOUT_FILENO, buf, n) != n)
        {
            perror("in write");
            close(f);
            exit(exit_status);
        }
        need_read_size -= n;
    }
    close(f);
    write(STDOUT_FILENO, "\n", 1);
}

static void
head_n(const char *file)
{
    char                buf[MAX_LINE_BUF_SIZE] = {0};
    int                 need_read_line = g_option.n;
    char                *p;
    FILE                *fp;
    int                 n;

    if((fp = fopen(file, "r")) == NULL){
        fprintf(stderr, "fopen file '%s' error: %s\n", file, strerror(errno));
    }

    while((need_read_line > 0) &&
          ((p = fgets(buf, MAX_LINE_BUF_SIZE, fp)) != NULL))
    {
        if(n = strlen(buf), write(STDOUT_FILENO, buf, n) != n)
        {
            perror("in write");
            exit(exit_status);
        }
        memset(buf, 0, MAX_LINE_BUF_SIZE);
        --need_read_line;
    }
}

static void
head(const char *file)
{
    assert(file != NULL && file[0] != 0);

    if(g_option.c != 0)
        head_c(file);
    else
        head_n(file);
}

int
main(int argc, char *argv[])
{
    int                 c;
    int                 option_index = 0;

    program_name = argv[0];
    atexit(head_exit);
    init_option();      /* 初始化参数       */

    /*长选项--(第一项:名字，第二项:是否带参数， 第三项:填0，第四项:短选项名)*/
    /*返回第四个数(如果第三个数为NULL,不然就返回0)*/
    static struct option long_options[] =
    {
        {"help", 0, 0, 'h'},
        {"bytes", 1, 0, 'b'},
        {"lines", 1, 0, 'l'},
        {0, 0, 0, 0}
    };

    /*短参数定义-*/
    static const char short_options[] = "c:n:H";

    while((c = getopt_long(argc, argv,
                           short_options, long_options, &option_index)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                head_usage_exit(EXIT_SUCESS);
                break;
            case 'c':
            case 'b':
                if (optarg && is_digit(optarg))
                    g_option.c = atoi(optarg);
                break;
            case 'n':
            case 'l':
                if (optarg && is_digit(optarg))
                    g_option.n = atoi(optarg);
                break;
            default:
                head_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    if (optind < argc)
        head(argv[optind]);
    else
        head_usage_exit(EXIT_FAILURE);

    exit_status= EXIT_SUCCESS;
    exit(exit_status);

}
