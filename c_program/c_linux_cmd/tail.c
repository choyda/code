/*
    tail.c
    功能:
        模拟錿tail命令
    历史记录:
        作者        时间        操作        联系方式
        卢富波      2014-05-13  创建        1164830775@qq.com
*/

#include "lib/command_comm.h"
#include "lib/list.h"
#include "lib/is_digit.h"
#include "lib/file_isexist.h"
#include "lib/safe-read.h"
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_BUF_SIZE        1024
#define MY_MIN(a,b)         (a)>(b)?(b):(a)

/*  描述选项信息    */
struct tail_option
{
    int             n;      /*  按行显示，默认为10行，只要为非0都是按行显示 */
    int             c;      /*  按字节显示，仅当n为0时，该选项才有作用      */
    int             f;      /*  实时监控，默认为0                           */
};



/*  描述链表节点信息    */
struct file_node
{
    char                *pname;     /*  指向文件名          */
    int                 fd;         /*  打开的文件描述符    */
    size_t              last_size;  /*  文件大小            */
    struct  list_head   list_node;  /*  链表节点            */
};


struct  tail_option     g_opt;
struct  list_head       g_head;


static void
init_option(struct tail_option *opt)
{
    opt->n = 10;
    opt->c = 0;
    opt->f = 0;
}



static void
tail_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s  FILE1 FILE2 ... [OPTION]\n", program_name);
        printf("-h  --help(--h) show the help infomation\n");
        printf("--bytes=N       display the last N bytes\n");
        printf("--lines=N       display the last N lines\n");
        printf("-f              monitor file change\n");
    }

    exit(status);
}

/*
    添加到列表
    返回值
    ==  0    成功
    >   0    操作系统的errno
    <   0    用户层出错
*/
static int
insert_list(char *pname)
{
    struct file_node        *pnode;
    int                     fd, save_errno;

    if(pname == NULL || pname[0] == 0)
        return -1;

    if(file_isreg(pname) == 0)
    {
        fprintf(stderr,
                "file [%s] is not a simple file\n",
                pname);
        return -2;
    }

    save_errno = errno;
    if((pnode = (struct file_node *)malloc(sizeof(struct file_node))) == NULL)
    {
        int         ret = errno;
        errno = save_errno;
        return ret;
    }

    if((fd = open(pname, O_RDONLY)) < 0)
    {
        int             ret = errno;
        errno = save_errno;
        fprintf(stderr,
                "open [%s] error [%s]\n",
                pname, strerror(ret));
        free(pnode);
        return ret;
    }

    pnode->pname = pname;
    pnode->fd = fd;
    pnode->last_size = 0;

    list_add_tail(&(pnode->list_node), &g_head);
    return 0;

}

static void
destroy_list()
{
    struct list_head            *tmp, *psafe;
    struct file_node            *pnode;

    list_for_each_safe(tmp, psafe, &g_head)
    {
        pnode = list_entry(tmp, struct file_node, list_node);
        close(pnode->fd);
        list_del(&(pnode->list_node));
        free(pnode);
    }

}

static void
tail_exit()
{
    destroy_list();
}

static int
pr_data(int from_fileno,        /*  从哪个文件描述符中打印        */
        int to_fileno,            /*  打印到哪个文件描述符          */
        size_t  offset            /*  开始打印的偏移量              */
       )
{
    char                buf[MAX_BUF_SIZE];
    int                 err;

    if(lseek(from_fileno, offset, SEEK_SET) == -1)
    {
        fprintf(stderr,
                "lseek [%d]error[%s]\n",
                from_fileno, strerror(errno));

        return errno;
    }

    while((err = safe_read(from_fileno, buf, MAX_BUF_SIZE)) > 0)
    {
        if(write(to_fileno, buf, err) != err)
        {
            fprintf(stderr,
                    "write [%d] error [%s]\n",
                    to_fileno, strerror(errno));

            return errno;
        }
    }

    return 0;
}

/*
    显示最后几行
*/
static int
tail_one_n(struct file_node *  pnode)
{
    char                    buf[MAX_BUF_SIZE];
    int                     i, offset_beg = pnode->last_size;
    int                     once_need_read;
    int                     readed_lines = 0;

    /*FIXME 以下的逻辑是否完美  */
    while(1)
    {
        once_need_read = MY_MIN(MAX_BUF_SIZE, offset_beg);  /*  此次需要读的大小    */
        assert(once_need_read > 0);
        if(lseek(pnode->fd, offset_beg-once_need_read, SEEK_SET) == -1)
        {
            fprintf(stderr, "in tail_one_n lseek error\n");

            return -1;
        }
        if(safe_read(pnode->fd, buf, once_need_read) != once_need_read)
        {
            fprintf(stderr, "read [%s] error\n", pnode->pname);

            return -2;
        }

        for(i = once_need_read - 1; i >= 0; --i)
        {

            if(buf[i] == '\n')
            {
                ++readed_lines;
                /*  已找到那么多行  */
                if(readed_lines == g_opt.n)
                {
                    printf("[%s] tail [%s] last [%d] lines\n", program_name, pnode->pname, g_opt.n);
                    return pr_data(pnode->fd, STDOUT_FILENO, offset_beg);
                }
            }

            --offset_beg;
        }

        /*  没有那么多行  */
        if(offset_beg == 0)
        {
            printf("[%s] tail [%s] last [%d] lines(all files)\n", program_name, pnode->pname, g_opt.n);
            return pr_data(pnode->fd, STDOUT_FILENO, offset_beg);
        }
    }

    assert(0);  /*  按想的思路是不应该走到这里的  */
    return -3;
}


/*
    显示最后几字节
*/
static int
tail_one_c(struct file_node * pnode)
{
    int             off_size = pnode->last_size;   /*  从文件开始处的偏移  */

    if(off_size > g_opt.c)
        off_size -= g_opt.c;

    printf("[%s] tail [%s] last [%d] bytes\n", program_name, pnode->pname, g_opt.c);
    return pr_data(pnode->fd, STDOUT_FILENO, off_size);
}

static int
tail_one(struct file_node * pnode)
{
    if((pnode->last_size= get_file_size(pnode->fd)) < 0)
    {
        fprintf(stderr,
                "get file [%s] size error\n",
                pnode->pname);

        return -1;
    }

    if(g_opt.n)
        return tail_one_n(pnode);
    else
        return tail_one_c(pnode);
}

static int
append(struct file_node * pnode)
{
    int                     now_size;

    if((now_size = get_file_size(pnode->fd)) < 0)
    {
        fprintf(stderr, "in append get size error\n");

        return -1;
    }

    if(now_size < pnode->last_size)
    {
        printf("file [%s] truncateed\n", pnode->pname);
        pnode->last_size = now_size;

        return 0;
    }
    else if(now_size == pnode->last_size)
    {

        return 0;
    }
    else
    {
        int ret = pr_data(pnode->fd, STDOUT_FILENO, pnode->last_size);
        pnode->last_size = now_size;

        return ret;
    }
}

static int
tail()
{
    int                         ret = 0;
    struct list_head            *tmp;
    struct file_node            *pnode;

    list_for_each(tmp, &g_head)
    {
        pnode = list_entry(tmp, struct file_node, list_node);
        ret |= tail_one(pnode);
        if(ret != 0)
            return ret;
    }

    if(g_opt.f)
    {
        while(1)
        {
            list_for_each(tmp, &g_head)
            {
                pnode = list_entry(tmp, struct file_node, list_node);
                ret |= append(pnode);
                if(ret != 0)
                    return ret;
            }
            sleep(1);
        }
    }

    return  0;

}

int
main(int argc, char *argv[])
{
    int                     c;

    INIT_LIST_HEAD(&g_head);
    atexit(tail_exit);
    init_option(&g_opt);
    exit_status = 0;
    static struct option const long_options[] =
    {
        {"help", 0, NULL, 'h'},
        {"bytes", 1, NULL, 'b'},
        {"lines", 1, NULL, 'l'},
        {"f", 1, NULL, 'f'},
        {NULL, 0, NULL, 0}
    };

    program_name = argv[0];
    while((c = getopt_long(argc, argv,
                           "b:l:hf", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                tail_usage_exit(EXIT_SUCESS);
                break;
            case 'l':
                if (optarg && is_digit(optarg, strlen(optarg)))
                    g_opt.n = atoi(optarg);
                break;
            case 'b':
                if (optarg && is_digit(optarg, strlen(optarg)))
                {
                    g_opt.c = atoi(optarg);
                    g_opt.n = 0;
                }
                break;

            case 'f':
                g_opt.f = 1;
                break;
            default:
                tail_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    if(optind == argc)
        tail_usage_exit(1);


    while(optind < argc)
        exit_status |= insert_list(argv[optind++]);

    if(exit_status != 0)
        exit(exit_status);

    exit_status = tail();
    exit(exit_status);
}

