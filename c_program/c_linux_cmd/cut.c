/*
    cut.c
    功能:
        模拟cut命令
    历史记录:
        作者        时间        操作        联系方式
        卢富波      2014-06-04  创建        1164830775@qq.com
*/

#include "lib/command_comm.h"
#include "lib/is_digit.h"
#include "lib/file_isexist.h"
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>

#define  VALUE_SEP          ","
#define  MY_MIN(a, b) ((a) > (b) ? (b) : (a))

struct value_set
{
    unsigned int        min;
    unsigned int        max;
};

struct cut_option
{
    char                d;      /* 分隔符            */
    struct value_set    f;      /* 显示的列          */
    struct value_set    c;      /* 显示第几个字节    */
};

static int
value_set_isseted(const struct value_set *v)
{
    return (v->min != (unsigned int)-1 &&
            v->max != (unsigned int)-1);
}

static void
init_value_set(struct value_set *v)
{
    v->min = (unsigned int)-1;
    v->max = (unsigned int)-1;
}

static void
init_opt(struct cut_option *op)
{
    op->d = '\t';
    init_value_set(&(op->f));
    init_value_set(&(op->c));
}

static void
fill_d(struct cut_option *op, const char *d)
{
    if(strlen(d) > 1)
    {
        fprintf(stderr,
                "only set one charactor\n");
        exit(1);
    }
    op->d = d[0];
}

static int
get_num(char *v)
{
    if(!is_digit(v, strlen(v)))
    {
        fprintf(stderr,
                "[%s] is not a number\n",
                v);
        exit(1);
    }

    return atoi(v);
}

static void
fill_value(struct value_set *val, const char *d)
{
    char        tmp[256];
    char        *token = NULL;

    strncpy(tmp, d, sizeof(tmp));
    /* min number */
    token = strtok(tmp, VALUE_SEP);
    if(token == NULL)
    {
        fprintf(stderr,
                "can't find first number in [%s]\n",
                d);
        exit(1);
    }
    val->min = get_num(token);

    /* max number */
    token = strtok(NULL, VALUE_SEP);
    if(token == NULL)
    {
        fprintf(stderr,
                "can't find second number in [%s]\n",
                d);
        exit(1);
    }
    val->max = get_num(token);

    if(val->max < val->min)
    {
        fprintf(stderr,
                "%d < %d\n",
                val->max, val->min);
        exit(1);
    }
}


static void
fill_f(struct cut_option *op, const char *d)
{
    fill_value(&(op->f), d);
}

static void
fill_c(struct cut_option *op, const char *d)
{
    fill_value(&(op->c), d);
}


static void
check_option(struct cut_option *op)
{
    if((value_set_isseted(&(op->c)) && value_set_isseted(&(op->f))) ||
       (!value_set_isseted(&(op->c)) && !value_set_isseted(&(op->f))))
    {
        fprintf(stderr,
                "-f -c only can set one and must be set one\n");
        exit(1);
    }
}

static void
cut_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s  [OPTION] FILE\n", program_name);
        printf("-d, --delimiter=DELIM   use DELIM instead of TAB for field delimiter\n");
        printf("-f, --fields=LIST       output  only these fields\n");
        printf("-c, --characters=LIST   output only these characters\n");
        printf("-h(-H)  --help          show help information\n");
        printf("\nNote if use -f or -c please use as below: -f2,6 \n");
    }

    exit(status);
}

static void
print_opt(struct cut_option *op)
{
    printf("sep[%c]\n", op->d);
    printf("c[%d:%d]\n",op->c.min, op->c.max);
    printf("f[%d:%d]\n",op->f.min, op->f.max);
}

static FILE*
open_simplefile(const char *file)
{
    FILE            *fp;

    if(!file_isreg(file))
    {
        fprintf(stderr,
                "file [%s] is not a simple file\n",
                file);
        exit(1);
    }

    if((fp = fopen(file, "r")) == NULL)
    {
        fprintf(stderr,
                "fopen [%s] error[%s]\n",
                file, strerror(errno));
        exit(1);
    }

    return fp;
}

static void
print_data(const char *data, size_t size)
{
    if(write(STDOUT_FILENO, data, size) != size)
        perror("write");
}

static void
print_data_n(const char *data, size_t size)
{
    print_data(data, size);
    if(write(STDOUT_FILENO, "\n", 1) != 1)
        perror("write");
}

static void
print_c(char *data, unsigned int min, unsigned int max)
{
    size_t              len = strlen(data);

    if(min >= len)
        return;

    len = MY_MIN(max-min, len-min);

    print_data_n(data+min, len);
}

static int
_cut_c(const struct cut_option *op, char *data)
{
    (void) print_c(data, op->c.min, op->c.max);
    return 0;
}

static int
_cut_f(const struct cut_option *op, char *data)
{
    char            *token;
    char            sep[2];
    unsigned int    index = 0;
#if 1 
    sprintf(sep, "%c", op->d);
    //strncpy(sep, &(op->d), 2);//note:这儿烤贝是错的，以后要注意到了，strncpy可能不带null
#else
    sep[0] = op->d;
    sep[1] = 0;
#endif
    token = strtok(data, sep);
    while(token != NULL)
    {
        if(index >= op->f.max)
            break;
        
        if(index >= op->f.min){
            print_data(token, strlen(token));
            print_data(" ", 1);
        }

        token = strtok(NULL, sep);
        ++index;
    }
    //print_data("\n", 1);
    return 0;
}

static int
_cut(const struct cut_option *op, char *data)
{
    if(value_set_isseted(&(op->c)))
        return _cut_c(op, data);

    return _cut_f(op, data);
}

static int
cut(struct cut_option *op, const char *file)
{
    char            tmpline[MAX_LINE_BUF_SIZE];
    FILE            *fp = open_simplefile(file);
    int             err;

    memset(tmpline, 0, MAX_LINE_BUF_SIZE);
    while(fgets(tmpline, MAX_LINE_BUF_SIZE, fp) != NULL)
    {
        if((err = _cut(op, tmpline)) != 0)
            return err;
        memset(tmpline, 0, MAX_LINE_BUF_SIZE);
    }

    if(ferror(fp))
    {
        fprintf(stderr,
                "fgets [%s] happend some error[%s]\n",
                file, strerror(errno));
        fclose(fp);
        exit(1);
    }

    fclose(fp);
    return 0;
}

int
main(int argc, char *argv[])
{
    int                     c;
    struct cut_option       g_option;

    static struct option const long_options[] =
    {
        {"help", 0, NULL, 'h'},
        {"delimiter", 1, NULL, 'd'},
        {"fields", 1, NULL, 'f'},
        {"characters", 1, NULL, 'c'},
        {NULL, 0, NULL, 0}
    };
    exit_status = 0;
    program_name = argv[0];
    init_opt(&g_option);
    while((c = getopt_long(argc, argv,
                           "hHd:f:c:", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                cut_usage_exit(EXIT_SUCESS);
                break;
            case 'd':
                if (optarg)
                    fill_d(&g_option, optarg);
                break;
            case 'f':
                if (optarg)
                    fill_f(&g_option, optarg);
                break;
            case 'c':
                if (optarg)
                    fill_c(&g_option, optarg);
                break;
            default:
                cut_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    check_option(&g_option);
    print_opt(&g_option);

    if(argc - optind != 1)
        cut_usage_exit(EXIT_FAILURE);

    exit_status = cut(&g_option, argv[optind]);

    exit(exit_status);
}


