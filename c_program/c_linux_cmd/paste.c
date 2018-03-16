/*
    功能:
        模拟paste命令  (只从两个文件读取)
    历史记录:
        作者        时间        操作        联系方式
        卢富波      2014-06-13  创建        1164830775@qq.com
*/
#include "lib/command_comm.h"
#include "lib/str_match.h"
#include "lib/char_map.h"
#include "lib/file_isexist.h"
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>


char            g_is_s = 0;
char            g_sep = '\t';

static void
paste_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s      [OPTION] FILE1 FILE2\n", program_name);
        printf("only read from FILE1 and FILE2\n");
        printf(" -d, --delimiters=LIST    reuse characters from LIST instead of TABs\n");
        printf(" -s, --serial             paste one file at a time instead of in parallel\n");
        printf("-h(-H)  --help            Show help information\n");
    }

    exit(status);
}

static void
pr_line( FILE *f)
{
    char            buf[MAX_LINE_BUF_SIZE];
    size_t          str_size;

    if(fgets(buf, MAX_LINE_BUF_SIZE, f) == NULL)
        return;

    str_size = strlen(buf);
    if(str_size && buf[str_size-1] == '\n')
        buf[str_size-1] = 0;

    fputs(buf, stdout);
}

static void
pr_without_n( FILE *f)
{
    while(!feof(f) && ferror(f))
    {
        pr_line(f);
        fputc(g_sep, stdout);
    }
    fputc('\n', stdout);
}

static int
paste_s( FILE *f1,  FILE *f2)
{
    pr_without_n(f1);
    pr_without_n(f2);

    return 0;
}

static int
paste_default( FILE *f1,  FILE *f2)
{
    while(!feof(f1) && !feof(f2) && !ferror(f1) && !ferror(f2))
    {
        pr_line(f1);
        fputc(g_sep, stdout);
        pr_line(f2);
        fputc('\n', stdout);
    }

    return 0;
}
static int
paste( char *file1,  char *file2)
{
    FILE                *f1, *f2;
    int                 rc;

    if(!file_isreg(file1))
    {
        pr_msg("file [%s] is not a simple fiele or had not existed\n", file1);
        return 1;
    }
    if(!file_isreg(file2))
    {
        pr_msg("file [%s] is not a simple fiele or had not existed\n", file2);
        return 1;
    }

    if((f1 = fopen(file1, "r")) == NULL)
    {
        pr_msg("fopen [%s] error[%s]\n", file1, strerror(errno));
        return 1;
    }
    if((f2 = fopen(file2, "r")) == NULL)
    {
        pr_msg("fopen [%s] error[%s]\n", file2, strerror(errno));
        fclose(f1);
        return 1;
    }

    if(g_is_s)
        rc = paste_s(f1, f2);
    else
        rc = paste_default(f1, f2);

    fclose(f1);
    fclose(f2);

    return rc;
}

int
main(int argc, char *argv[])
{
    int                 c;

    static struct option const long_options[] =
    {
        {"help", 0, NULL, 'h'},
        {"delimiters", 1, NULL, 'd'},
        {"serial", 0, NULL, 's'},
        {NULL, 0, NULL, 0}
    };
    exit_status = EXIT_SUCESS;
    program_name = argv[0];
    while((c = getopt_long(argc, argv,
                           "hHd:s", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                paste_usage_exit(EXIT_SUCESS);
                break;
            case 's':
                g_is_s = (char)1;
                break;
            case 'd':
                if (optarg)
                    g_sep = optarg[0];
                break;
            default:
                paste_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    if(argc - optind != 2)
        paste_usage_exit(EXIT_FAILURE);

    if(paste(argv[optind], argv[optind+1]))
        exit_status = EXIT_FAILURE;

    exit(exit_status);
}

