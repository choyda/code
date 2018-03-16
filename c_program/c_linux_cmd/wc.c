/*
    wc.c
    功能:
        模拟wc命令
    历史记录:
        作者        时间        操作        联系方式
        卢富波      2014-06-03  创建        1164830775@qq.com
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

struct wc_option
{
    unsigned char           c;      /*  显示字节数  */
    unsigned char           l;      /*  显示行数    */
    unsigned char           w;      /*  显示单词数  */
};

struct wc_count
{
    unsigned int            c;
    unsigned int            l;
    unsigned int            w;
};

struct wc_option            g_option;
struct wc_count             g_count;

static void
init_option()
{
    memset(&g_option, 0, sizeof(g_option));
}

static void
wc_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s [OPTION] FILELIST\n", program_name);
        printf("-c, --chars  print the character counts\n");
        printf("-l, --lines  print the new line counts\n");
        printf("-w, --words  print the word counts\n");
        printf("-h(-H)  --help  show help information\n");
    }

    exit(status);
}

static int
char_is_in(char c, const char *str, size_t len)
{
    size_t                  i;

    for(i = 0; i < len; ++i)
        if(str[i] == c)
            return 1;

    return 0;
}


static unsigned int
getwordcount(char *buf)
{
    const char              *word_sep = " \r\f\t\v\n";
    unsigned int            word_count = 0;
    char                    *token;
    size_t                  total_size = strlen(buf);

    if(buf == NULL || buf[0] == 0)
        return 0;
    /*  去前面的无关字符    */
    {
        while(total_size > 0 && char_is_in(buf[0], word_sep, strlen(word_sep)))
        {
            --total_size;
            buf++;
        }

    }

    /*  去掉后面的无关字符  */
    {
        while(total_size > 0 && char_is_in(buf[total_size], word_sep, strlen(word_sep)))
        {
            buf[total_size] = 0;
            --total_size;
        }
    }

    token = strtok(buf, word_sep);
    while(token)
    {
        ++word_count;
        token = strtok(NULL, word_sep);
    }

    return word_count;
}

static void
pr_wc(const char *file)
{
    if(g_option.c == 0 && g_option.l == 0 && g_option.w == 0)
    {
        printf("%u\t%u\t%u\t%s\n", g_count.l, g_count.w, g_count.c, file);
        return;
    }

    if(g_option.l != 0)
        printf("%u\t", g_count.l);
    if(g_option.w != 0)
        printf("%u\t", g_count.w);
    if(g_option.c != 0)
        printf("%u\t", g_count.c);

    printf("%s\n", file);
}

static int
wc(const char *file)
{
    FILE            *fp;
    char            buf[MAX_LINE_BUF_SIZE];

    if(!file_isreg(file))
    {
        pr_msg("file [%s] is not access or is not a simple file\n", file);
        return 1;
    }

    if((fp = fopen(file, "r")) == NULL)
    {
        pr_msg("open file [%s] error [%s]\n", file, strerror(errno));
        return 1;
    }

    memset(&g_count, 0, sizeof(g_count));
    while(fgets(buf, MAX_LINE_BUF_SIZE, fp) != NULL)
    {
        ++g_count.l;
        g_count.c += strlen(buf);
        g_count.w += getwordcount(buf);
    }

    if(ferror(fp))
    {
        pr_msg("fgets [%s] error [%s]\n", file, strerror(errno));
        return 1;
    }

    pr_wc(file);
    return 0;
}

int
main(int argc, char *argv[])
{
    int                 c;
    static struct option const long_options[] =
    {
        {"help", 0, NULL, 'h'},
        {"words", 0, NULL, 'w'},
        {"lines", 0, NULL, 'l'},
        {"chars", 0, NULL, 'c'},
        {NULL, 0, NULL, 0}
    };
    exit_status = 0;
    program_name = argv[0];
    init_option();
    while((c = getopt_long(argc, argv,
                           "hHwlc", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                wc_usage_exit(EXIT_SUCESS);
                break;
            case 'w':
                g_option.w = 1;
                break;
            case 'l':
                g_option.l = 1;
                break;
            case 'c':
                g_option.c = 1;
                break;
            default:
                wc_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    if(argc == optind )
        wc_usage_exit(EXIT_FAILURE);

    while(optind < argc)
        if(wc(argv[optind++]) != 0)
            exit_status = EXIT_FAILURE;

    exit(exit_status);
}

