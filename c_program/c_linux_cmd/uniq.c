/*
    功能:
        模拟uniq命令
    历史记录:
        作者        时间        操作        联系方式
        卢富波      2014-06-08  创建        1164830775@qq.com
    FIXME
        仅仅实现从文件中读取文件来处理,与标准的uniq一样，输入的数据需要先排序
*/
#include "lib/command_comm.h"
#include "lib/err_msg.h"
#include "lib/is_digit.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define MAX_LINE_NUM 8192           /*  能处理的最大行数    */

struct uniq_option
{
    unsigned char               c;  /*  是否打印重复次数    */
    unsigned char               d;  /*  是否只打印重复行    */
    unsigned char               f;  /*  忽略的前n个区       */
    unsigned char               i;  /*  是否忽略大小写      */
    unsigned short              s;  /*  忽略前n个字节       */
    char                        p;  /*  分隔符              */
};

struct one_line
{
    const char              *base;
    size_t                  size;
    size_t                  repeated_num;
};

struct all_data
{
    struct one_line         dataarr[MAX_LINE_NUM];
    size_t                  used;
};

struct uniq_option              g_option;
struct all_data                 g_data;

static void
init_data()
{
    memset(&g_data, 0, sizeof(g_data));
}

static size_t
get_simfile_size(const char *file)
{
    struct stat         stabuf;

    if(stat(file, &stabuf) != 0)
    {
        fprintf(stderr,
                "stat [%s] error[%s]\n",
                file, strerror(errno));
        exit(1);
    }

    if(!S_ISREG(stabuf.st_mode))
    {
        fprintf(stderr,
                "file [%s] is not a simple file\n",
                file);
        exit(1);
    }

    return stabuf.st_size;
}

static void
get_field_by_field(char *buf)
{
    char            sep[2];
    char            *token = buf;
    int             field_num = (int)g_option.f;
    char            *outer_ptr=NULL;

    sprintf(sep, "%c", g_option.p);
    while((token = strtok_r(buf, sep, &outer_ptr)) != NULL && field_num-- > 0)
    {
        buf = NULL;
    }

    if(token)
    {
        memmove(buf, token, strlen(token));
        buf[strlen(token)] = 0;
    }
    else
        buf[0] = 0;
}

static void
get_field_by_char(char *buf)
{
    if(g_option.s < MAX_LINE_BUF_SIZE)
        memmove(buf, buf+g_option.s, MAX_LINE_BUF_SIZE-g_option.s);
    else
        buf[0] = 0;
}

static void
get_field(char *buf)
{
    if(g_option.s)
        get_field_by_char(buf);
    else
        get_field_by_field(buf);
}

static int
cmptolast(const char * buf, const char *last)
{
    int         rc;
    char        buf1[MAX_LINE_BUF_SIZE+1], buf2[MAX_LINE_BUF_SIZE+1];

    memset(buf1, 0, sizeof(buf1));
    memset(buf2, 0, sizeof(buf2));

    snprintf(buf1, MAX_LINE_BUF_SIZE, "%s", buf);
    snprintf(buf2, MAX_LINE_BUF_SIZE, "%s", last);

    /*更新buf1, buf2中内容为需要比较的列 */
    get_field(buf1);
    get_field(buf2);

    if(g_option.i)
        rc = strcasecmp(buf1, buf2);
    else
        rc = strcmp(buf1, buf2);

    return rc;
}

static void
init_one(const char *token, unsigned int index)
{
    g_data.dataarr[index].base = token;
    g_data.dataarr[index].size = strlen(token);
    g_data.dataarr[index].repeated_num = 1;

    g_data.used++;
}

static int
fill_line( const char *token)
{
    size_t              index = g_data.used;

    if(index == MAX_LINE_NUM)

        return 1;

    if(index == 0)
    {
        init_one(token, 0);
        return 0;
    }

    if(cmptolast(token, g_data.dataarr[index-1].base) != 0)
        init_one(token, index);
    else
        g_data.dataarr[index-1].repeated_num++;

    return 0;
}

static int
fill_data(char *pdata)
{
    const char          *sep = "\n";
    char                *token;
    int                 err;

    init_data();
    token = strtok(pdata, sep);
    while(token != NULL)
    {
        if((err = fill_line(token)) != 0)
        {
            printf("fill_line[%s] error[%d]\n", token, err);
            break;
        }

        token = strtok(NULL, sep);
    }

    return 0;
}

static void
init_option()
{
    memset(&g_option, 0, sizeof(g_option));
    g_option.p = '\t';
}

static void
adjust_option()
{
    if(g_option.f != 0 && g_option.s != 0)
        g_option.s = 0;
}

static void
uniq_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s                   [OPTION] FILE\n", program_name);
        printf("-c, --count                 prefix lines by the number of occurrences \n");
        printf("-d, --repeated              only print duplicate lines\n");
        printf("-f, --skip-fields=N         avoid comparing the first N fields\n");
        printf("-i, --ignore-case           ignore differences in case when comparing\n");
        printf("-s, --skip-chars=N          avoid comparing the first N characters\n");
        printf("-p, --separater=CHAR        separater, default '\\t'\n");
        printf("-h(-H)  --help              Show help information\n");
    }

    exit(status);
}

static void
fill_p(const char *str)
{
    if(strlen(str) > 1)
        pr_msg2exit(1, "[%s]is error, only set one charactor\n", str);

    g_option.p = str[0];
}

static void
fill_f(char *str)
{
    if(!is_digit(str, strlen(str)))
        pr_msg2exit(1, "[%s] is not a number\n", str);

    g_option.f = atoi(str);
}

static void
fill_s(char *str)
{
    if(!is_digit(str, strlen(str)))
        pr_msg2exit(1, "[%s] is not a number\n", str);

    g_option.s = atoi(str);
}

FILE *
open_for_read(const char *file)
{
    FILE            *fp;

    if((fp = fopen(file, "r")) == NULL)
    {
        pr_msg2exit(EXIT_FAILURE,
                    "open file[%s] error[%s]\n",
                    file, strerror(errno));
    }

    return fp;
}



static void
pr_uniqddata()
{
    int             i;

    for(i = 0; i < g_data.used; ++i)
    {
        printf("%s\n", g_data.dataarr[i].base);
    }

}


static int
uniq(const char*file)
{
    size_t              filesize = get_simfile_size(file);
    char                *pmapfile = NULL;
    int                 fd = -1, err = 0;

    if((fd = open(file, O_RDONLY)) < 0)
    {
        perror("open");
        exit(1);
    }

    if((pmapfile = (char *)mmap(0, filesize, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }
    if((err = fill_data(pmapfile)) != 0)
        goto end;

    pr_uniqddata();
end:
    if(pmapfile != NULL)
        munmap(pmapfile, filesize);
    if(fd != -1)
        close(fd);

    return err;
}


int
main(int argc, char *argv[])
{
    int                 c;

    static struct option const long_options[] =
    {
        {"help", 0, NULL, 'h'},
        {"count", 0, NULL, 'c'},
        {"repeated", 0, NULL, 'd'},
        {"skip-fields", 1, NULL, 'f'},
        {"ignore-case", 0, NULL, 'i'},
        {"skip-chars", 1, NULL, 's'},
        {"separater", 1, NULL, 'p'},
        {NULL, 0, NULL, 0}
    };
    init_option(&g_option);
    exit_status = EXIT_SUCESS;
    program_name = argv[0];
    while((c = getopt_long(argc, argv,
                           "hHcdf:is:p:", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                uniq_usage_exit(EXIT_SUCESS);
                break;
            case 'c':
                g_option.c = 1;
                break;
            case 'd':
                g_option.d = 1;
                break;
            case 'f':
                if(optarg)
                    fill_f(optarg);
                break;
            case 'i':
                g_option.i = 1;
                break;
            case 's':
                if(optarg)
                    fill_s(optarg);
                break;
            case 'p':
                if(optarg)
                    fill_p(optarg);
                break;
            default:
                uniq_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    if(argc - optind != 1)
        uniq_usage_exit(EXIT_FAILURE);

    adjust_option();
    if(uniq(argv[optind]) != 0)
        exit_status = EXIT_FAILURE;

    exit(exit_status);
}





