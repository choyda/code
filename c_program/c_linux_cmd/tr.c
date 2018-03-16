/*
    功能:
        模拟tr命令  (替换和删除,标准输入作为参数)
    历史记录:
        作者        时间        操作        联系方式
        卢富波      2014-06-12  创建        1164830775@qq.com
*/
#include "lib/command_comm.h"
#include "lib/str_match.h"
#include "lib/char_map.h"
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>


char                    *g_del_str = NULL;

static void
tr_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s       SET1 [SET2]\n", program_name);
        printf("only read from stdin\n");
        printf("-d,--delete     Delete lines that had SET1 strings\n");
        printf("-h(-H)  --help  Show help information\n");
    }

    exit(status);
}

static int
is_exclusive(char *line, char *finded)
{
    return !is_matched(finded, line);
}

static int
tr_del(char *str_del)
{
    char            buf[MAX_LINE_BUF_SIZE];

    while(fgets(buf, MAX_LINE_BUF_SIZE, stdin) != NULL)
        if(is_exclusive(buf, str_del))
            fputs(buf, stdout);
    if(ferror(stdin))
        pr_msg2exit(errno, "stdin error\n");
    if(ferror(stdout))
        pr_msg2exit(errno, "stdout error\n");

    return 0;
}

static char
finded_map(char finded_char, struct char_map *c_map)
{
    size_t           i;

    for(i = 0; i < c_map->size; ++i)
        if(finded_char == c_map->map_ral[i].map_src)
            return c_map->map_ral[i].map_dst;

    return 0;
}

static void
replace_char(char *line, struct char_map *c_map)
{
    char                rep_char;

    for( ; line[0] != 0; ++line)
        if((rep_char = finded_map(line[0], c_map)) != 0)
            line[0] = rep_char;
}

static int
tr_replace(char *replace_src, char *replace_dst)
{
    struct char_map         c_map;
    char                    buf[MAX_LINE_BUF_SIZE];
    int                     rc;

    if((rc = fill_map(replace_src, replace_dst, &c_map)) != 0)
        return rc;

    while(fgets(buf, MAX_LINE_BUF_SIZE, stdin) != NULL)
    {
        replace_char(buf, &c_map);
        fputs(buf, stdout);
    }

    if(ferror(stdin))
        pr_msg2exit(errno, "stdin error\n");
    if(ferror(stdout))
        pr_msg2exit(errno, "stdout error\n");

    return 0;
}

int
main(int argc, char *argv[])
{
    int                 c;

    static struct option const long_options[] =
    {
        {"help", 0, NULL, 'h'},
        {"delete", 1, NULL, 'd'},
        {NULL, 0, NULL, 0}
    };
    exit_status = EXIT_SUCESS;
    program_name = argv[0];
    while((c = getopt_long(argc, argv,
                           "hHd:", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                tr_usage_exit(EXIT_SUCESS);
                break;
            case 'd':
                if (optarg)
                    g_del_str = optarg;
                break;
            default:
                tr_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    if(g_del_str != NULL && argc - optind != 0)
        tr_usage_exit(EXIT_FAILURE);

    if(g_del_str == NULL && argc - optind != 2)
        tr_usage_exit(EXIT_FAILURE);

    if(g_del_str)
        exit_status = tr_del(g_del_str);
    else
        exit_status = tr_replace(argv[optind], argv[optind+1]);


    exit(exit_status);
}


