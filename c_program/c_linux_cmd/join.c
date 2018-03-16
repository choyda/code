/*
    join.c
    功能:
        模拟join命令
    历史记录:
        作者        时间        操作        联系方式
        卢富波      2014-06-16  创建        1164830775@qq.com
*/

#include "lib/command_comm.h"
#include "lib/is_digit.h"
#include "lib/err_msg.h"
#include "lib/file_isexist.h"
#include "lib/get_filesize.h"
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

#define     SEPARATOR   ' '
#define     MAX_LINE    1024


struct join_option
{
    char            t;  /*  分隔符              */
    unsigned char   k1; /*  第一个文件的列主键  */
    unsigned char   k2; /*  第一个文件的列主键  */
};

struct line
{
    char            *key_begin;
    char            *key_end;
    char            *line_begin;
    size_t          line_size;
};

struct file
{
    struct line     line[MAX_LINE];
    size_t          used;
};

struct join_option          g_option;
struct file                 g_f1;
struct file                 g_f2;

static void
init_option()
{
    g_option.t = SEPARATOR;
    g_option.k1 = 1;
    g_option.k2 = 1;
}

static void
init_file(struct file *f)
{
    if(f)
        memset(f, 0, sizeof(struct file));
}

static void
join_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s  FILE1 FILE2 [OPTION]\n", program_name);
        printf("-tCHAR                  use CHAR as input and output field separator\n");
        printf("-1FIELD                 join on this FIELD of FILE1\n");
        printf("-2FIELD                 join on this FIELD of FILE2\n");
        printf("-h  --help(--h)         show the help infomation\n");
    }

    exit(status);
}

static void
check(const char *file)
{
    if(!file_isreg(file))
        pr_msg2exit(EXIT_FAILURE, "file[%s] is not a simple file or had not existed\n", file);
}
static char *
my_map(const char *file, size_t *filesize, int *fd)
{
    char                *pmapfile = NULL;

    *filesize = get_filesize(file);
    if((*fd = open(file, O_RDONLY)) < 0)
        pr_msg2exit(EXIT_FAILURE, "open [%s] error[%s]\n", file, strerror(errno));

    if((pmapfile = (char *)mmap(0, *filesize, PROT_READ|PROT_WRITE, MAP_PRIVATE, *fd, 0)) == MAP_FAILED)
        pr_msg2exit(EXIT_FAILURE, "mmap [%s] error[%s]\n", file, strerror(errno));

    return pmapfile;
}

char *
offset_key(char *buf, int field_num)
{
    char            sep[2];
    char            *token = buf;
    char            *outer_ptr=NULL;

    sprintf(sep, "%c", g_option.t);
    while((token = strtok_r(buf, sep, &outer_ptr)) != NULL && --field_num > 0)
    {
        buf = NULL;
    }

    return token;
}

static void
fill_line(char *buf, size_t size, int field_num, struct line *line)
{
    char            *tmpbuf = (char *)malloc(size+1);
    char            *off;

    if(tmpbuf == NULL)
        pr_msg2exit(EXIT_FAILURE, "malloc error[%s]\n", strerror(errno));
    sprintf(tmpbuf, "%s", buf);

    line->line_begin = buf;
    line->line_size = size;

    if((off = offset_key(tmpbuf, field_num)) == NULL)
        goto end;

    line->key_begin = buf+(off-tmpbuf);
    line->key_end = line->key_begin + strlen(off);
    assert(line->key_end >= line->key_begin);

end:
    if(tmpbuf)
        free(tmpbuf);
}

static int
add_line(struct file *f, char *line_buf, size_t size, unsigned char key_index)
{
    size_t              index = f->used;

    if(index == MAX_LINE-1)
        return 1;

    fill_line(line_buf, size, key_index, &(f->line[index]));

    ++(f->used);

    return 0;
}

static int
create_file(char *data, struct file *f, unsigned char key_index)
{
    const char          *sep = "\n";
    char                *token;

    init_file(f);
    token = strtok(data, sep);
    while(token != NULL)
    {
        if(add_line(f, token, strlen(token), key_index))
            return 1;
        token = strtok(NULL, sep);
    }

    return 0;
}

static int
fields_equal(struct line *l, struct line *r)
{
    size_t              l_size = l->key_end - l->key_begin;
    size_t              r_size = r->key_end - r->key_begin;

    if( l->key_begin != NULL &&
        r->key_begin != NULL &&
        l_size == r_size)
    {
        if(memcmp(l->key_begin, r->key_begin, l_size) == 0)
            return 1;
    }

    return 0;
}

static void
pr_data1(char *begin, char *end)
{
    size_t          write_size = end - begin;

    if(end > begin)
        if(write(STDOUT_FILENO, begin, write_size) != write_size)
            pr_msg2exit(EXIT_FAILURE, "write errror[%s]\n", strerror(errno));
}

static void
pr_data2(char *data, size_t size)
{
    if(write(STDOUT_FILENO, data, size) != size)
        pr_msg2exit(EXIT_FAILURE, "write errror[%s]\n", strerror(errno));
}

static void
pr_joined(struct line *l, struct line *r)
{
    if(!fields_equal(l, r))
        return;

    pr_data1(l->key_begin, l->key_end);
    pr_data2(&g_option.t, 1);
    pr_data1(l->line_begin, l->key_begin);
    pr_data1(l->key_end, l->line_begin + l->line_size);
    pr_data2(&g_option.t, 1);
    pr_data1(r->line_begin, r->key_begin);
    pr_data1(r->key_end, r->line_begin + r->line_size);
    pr_data2("\n", 1);
}

static void
pr_join()
{
    size_t                  i, j;

    for(i = 0; i < g_f1.used; ++i)
        for(j = 0; j < g_f2.used; ++j)
            pr_joined(&g_f1.line[i], &g_f2.line[j]);
}

static int
_join(char *f1, char *f2)
{
    int             err;

    if((err = create_file(f1, &g_f1, g_option.k1)) != 0)
        return err;
    if((err = create_file(f2, &g_f2, g_option.k2)) != 0)
        return err;

    pr_join();

    return 0;
}

static int
join(const char *file1, const char *file2)
{
    char            *f1, *f2;
    size_t          s1, s2;
    int             fd1, fd2;
    int             rc;

    check(file1);
    check(file2);
    f1 = my_map(file1, &s1, &fd1);
    f2 = my_map(file2, &s2, &fd2);

    rc = _join(f1, f2);

    munmap(f1, s1);
    munmap(f2, s2);
    close(fd1);
    close(fd2);

    return rc;
}


int
main(int argc, char *argv[])
{
    int                     c;

    static struct option const long_options[] =
    {
        {"help", 0, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };

    exit_status = EXIT_SUCESS;
    program_name = argv[0];
    init_option(&g_option);
    while((c = getopt_long(argc, argv,
                           "hHt:1:2:", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                join_usage_exit(EXIT_SUCESS);
                break;
            case 't':
                if(optarg)
                    g_option.t = optarg[0];
                break;
            case '1':
                if(optarg && is_digit(optarg, strlen(optarg)))
                    g_option.k1 = atoi(optarg);
                break;
            case '2':
                if(optarg && is_digit(optarg, strlen(optarg)))
                    g_option.k2 = atoi(optarg);
                break;
            default:
                join_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    if(argc - optind != 2)
        join_usage_exit(EXIT_FAILURE);

    if(join(argv[optind], argv[optind+1]))
        exit_status = EXIT_FAILURE;

    exit(exit_status);
}


