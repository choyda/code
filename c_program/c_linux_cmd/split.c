/*
    功能:
        模拟split命令  (分割文件)
    历史记录:
        作者        时间        操作        联系方式
        卢富波      2014-06-16  创建        1164830775@qq.com

    FIXME:
        这个写的太粗糙了，只实现了功能
*/
#include "lib/command_comm.h"
#include "lib/err_msg.h"
#include "lib/file_isexist.h"
#include "lib/err_msg.h"
#include "lib/is_digit.h"
#include "lib/safe-read.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

size_t              g_btypes = 0;
size_t              g_line = 0;
int                 g_suffix = 0;
FILE                *g_tofile = NULL;

static void
split_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s    [OPTION]   FILE\n", program_name);
        printf("-l, --lines=NUMBER  Put NUMBER lines per output file\n");
        printf("-b, --bytes=SIZE    Put SIZE bytes per output file\n");
        printf("Note:               The suffix is based on the interger\n");
        printf("-h(-H)  --help      Show help information\n");
    }

    exit(status);
}

static void
check_param()
{
    if( g_btypes == 0 &&
        g_line == 0)
        pr_msg2exit(EXIT_FAILURE, "You must set -b or -l\n");

    if( g_btypes &&
        g_line)
        pr_msg2exit(EXIT_FAILURE, "The -b and -l only can set one\n");
}

static FILE*
_open(const char *f, const char *mod)
{
    FILE            *fp;

    if((fp = fopen(f, mod)) == NULL)
        pr_msg2exit(EXIT_FAILURE, "open [%s] error[%s]\n", f, strerror(errno));

    return fp;
}

static FILE*
new_open(const char *file)
{
#define LOCAL_FILE_NAME 256
    char            tmp[256+32];
    char            suffix[32];

    memset(tmp, 0, sizeof(tmp));
    memset(suffix, 0, sizeof(suffix));
    snprintf(tmp, LOCAL_FILE_NAME, "%s", file);
#undef LOCAL_FILE_NAME

    sprintf(suffix, "%03d", g_suffix++);
    strcat(tmp, suffix);

    if(file_isexist(tmp))
        pr_msg2exit(EXIT_FAILURE, "file [%s] had existed\n", tmp);

    return _open(tmp, "w");
}

static int
new_open2(const char *file)
{
    return fileno(new_open(file));
}

static int
write_line(const char *buf, size_t line_no, const char *file)
{
    if(line_no%g_line == 0)
    {
        if(g_tofile)
            fclose(g_tofile);
        g_tofile = new_open(file);
    }

    if(fputs(buf, g_tofile) == EOF)
        return 1;

    return 0;
}

static int
split_bytes(const char *file)
{
    int             fd, fd_to = -1;
    char            *buf = (char *)malloc(g_btypes);
    int             readed_size;

    if(buf == NULL)
        pr_msg2exit(EXIT_FAILURE, "malloc error\n");
    if((fd = open(file, O_RDONLY)) < 0)
    {
        free(buf);
        pr_msg2exit(EXIT_FAILURE, "open [%s] error[%s]\n", file, strerror(errno));
    }

    while((readed_size = safe_read(fd, buf, g_btypes)) > 0)
    {
        if(fd_to != -1)
            close(fd_to);
        fd_to = new_open2(file);
        write(fd_to, buf, readed_size);
    }

    if(fd_to != -1)
        close(fd_to);

    close(fd);
    return 0;
}

static int
split_lines(const char *file)
{
    char            buf[MAX_LINE_BUF_SIZE];
    FILE            *fp = _open(file, "r");
    size_t          line_no = 0;

    while(fgets(buf, MAX_LINE_BUF_SIZE, fp) != NULL)
    {
        if(write_line(buf, line_no++, file) != 0)
        {
            fclose(fp);
            return 1;
        }
    }

    if(ferror(fp))
    {
        fclose(fp);
        pr_msg2exit(EXIT_FAILURE, "fgets error\n");
    }

    if(g_tofile)
        fclose(g_tofile);

    fclose(fp);
    return 0;
}

static int
split(const char *file)
{
    if(!file_isreg(file))
        pr_msg2exit(EXIT_FAILURE, "file [%s] is not a common file or does not exist\n");

    if(g_btypes)
        return split_bytes(file);

    return split_lines(file);
}
int
main(int argc, char *argv[])
{
    int                 c;

    static struct option const long_options[] =
    {
        {"help", 0, NULL, 'h'},
        {"lines", 1, NULL, 'l'},
        {"bytes", 1, NULL, 'b'},
        {NULL, 0, NULL, 0}
    };
    exit_status = EXIT_SUCESS;
    program_name = argv[0];
    while((c = getopt_long(argc, argv,
                           "hHl:b:", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                split_usage_exit(EXIT_SUCESS);
                break;
            case 'l':
                if(optarg)
                {
                    if(!is_digit(optarg, strlen(optarg)))
                        pr_msg2exit(EXIT_FAILURE, "[%s] is not a integer\n", optarg);
                    g_line = atoi(optarg);
                }
                break;
            case 'b':
                if(optarg)
                {
                    if(!is_digit(optarg, strlen(optarg)))
                        pr_msg2exit(EXIT_FAILURE, "[%s] is not a integer\n", optarg);
                    g_btypes= atoi(optarg);
                }
                break;
            default:
                split_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    if(argc - optind != 1)
        split_usage_exit(EXIT_FAILURE);

    check_param();

    if(split(argv[optind]) != 0)
        exit_status = EXIT_FAILURE;

    exit(exit_status);
}


