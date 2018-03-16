/*
    该文件简单模拟linux系统的ls命令(实现了大部份功能)

历史记录:
    日期        作者        描述        联系方式
    2014.04.14  卢富波      首次创建    164830775@qq.com

功能需求(FIXME)，只做了个人觉得常用的选项:
    --help(--h) -h      只显示帮助信息，不进行任何其它动作，其它参数作废
    -a      显示所有文件(即包括隐藏文件)
    -l      显示详细信息
    -L      只打印链接文件名
    -r      反排序
    -R      递归显示,即遇到目录会再次进入目录把文件列举出来
    -c      以属性最后修改时间排序
    -t      以修改时间排序
    -u      以创建时间排序
    -S      以文件大小排序
    -n      以文件名排序(以文件名排序，是默认排序，如果有多种排序也只采用文件名排序)
    -i      显示I节点
    -d      显示设备序号
    -b      显示块大小
    -C      显示占多少块

FIXME:
    已知BUG:
    (1) 打印出来的文件大小，我是打印的实际文件大小(个人偏好)，标准的是打印的块大小*块数
*/

#include <time.h>
#include <stdio.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <assert.h>
#include <getopt.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include "lib/list.h"


#define MAX_PATH            256             /*文件或者目录名的最大长度*/
#define PEER_MALLOC_FILE    64              /*当内存不够时，最少申请多少个struct file_info的大小
                                            避免频繁申请内存*/

/*一个文件基本信息的节点*/
struct file_info
{
    char                fil_name[MAX_PATH];     /* 文件名节点 */
    char                dst_name[MAX_PATH];     /* 链接文件名真实名 */
    char                is_link;                /* 是否是链接文件 1:是 0:否 */
    struct stat         statbuf;                /*文件属性节点*/
};

/*一个目录的信息列表*/
struct dir_info
{
    char                dir_path[MAX_PATH];     /*目录名节点*/
    struct file_info    *p_filenode;             /*指向该目录下的文件信息
                                                内存类似于数组，之所以不采用链表,是为了好用qsort进行排序*/
    size_t              used_size;              /*已用了多少个strcut file_info*/
    size_t              free_size;              /*空余还有多少个strcut file_info*/
    size_t              dir_size;               /*该目录的大小(单位为k),用ls -l显示的第一行 total(总用量)的值*/
    char                need_print_total;       /*该目录是否需要打印total*/
};

/*等待遍历节点的节点*/
struct dir_list
{
    char                dir_path[MAX_PATH];     /*待遍历的目录节点*/
    struct list_head    list_node;              /*链表节点*/
};

struct param
{
    char            a;      /* 显示所有文件 */
    char            l;      /* 显示详细信息 */
    char            L;      /* 只显示链接文件 */
    char            r;      /* 反排序 */
    char            R;      /* 递归显示 */
    char            c;      /* 状态修改时间排序*/
    char            t;      /* 最后修改时间排序*/
    char            u;      /* 最后访问时间排序*/
    char            S;      /* 以文件大小排序*/
    char            n;      /* 以文件名排序*/
    char            i;      /* 显示I节点*/
    char            d;      /* 显示每块大小*/
    char            C;      /* 显示占了多少块*/
    char            h;      /* 显示帮助信息*/
};

typedef int (*COM_FUNC)(const void *, const void *);

struct dir_info         g_dir_info;             /*当前正处理的目录*/
struct param            g_param;                /*传入参数*/
struct list_head        g_dir_head;             /*需要扫描目录的链表头*/
COM_FUNC                g_com_func = NULL;

static void
printf_usage()
{
    printf("usage:\n");
    printf("-h  --help(--h) show the help infomation\n");
    printf("-a              show all files\n");
    printf("-l              show the detailed information\n");
    printf("-L              only show linkname if the file is linkfile\n");
    printf("-r              recursive display\n");
    printf("-c              sort by status change\n");
    printf("-t              sort by last modify time\n");
    printf("-u              sort by last access time\n");
    printf("-S              sort by file size\n");
    printf("-n              sort by file name, the default.And if input more than one sort way, only sort by file name\n");
    printf("-i              show i-node info\n");
    printf("-d              show I/O block size\n");
    printf("-C              show number of disk blocks\n");
}

/*计算一个目录的大小，即ls -l显示出来的第一行 "total" */
static void
cal_dir_size()
{
    size_t              i;
    struct file_info    *p;

    g_dir_info.dir_size = 0;
    g_dir_info.need_print_total = 1;    /*需要打印total字段标志*/
    for(i = 0; i < g_dir_info.used_size; ++i)
    {
        p = g_dir_info.p_filenode + i;
        g_dir_info.dir_size += p->statbuf.st_blksize * p->statbuf.st_blocks;
    }
    /*换算为k*/
    g_dir_info.dir_size = g_dir_info.dir_size/1024/8;
}
/*
    程序带错误码退出
*/
static void
dead_errno(int no)
{
    assert(0);
    exit(no);
}

/*FIXME 内存追加方式申请内存*/
static void
realloc_file_info(struct dir_info *dir_info)
{
    assert(dir_info != NULL);
    assert(dir_info->free_size == 0);   /*没空闲内存了才会申请*/
    size_t              newsize;        /*新内存的大小*/

    newsize = (dir_info->used_size + PEER_MALLOC_FILE)*sizeof(struct file_info);
    if((dir_info->p_filenode = realloc(dir_info->p_filenode, newsize)) == NULL)
    {
        perror("realloc");
        dead_errno(1);
    }

    dir_info->free_size += PEER_MALLOC_FILE;
}


static void
save_file_info(struct dir_info *dir_info, struct stat *stat, char *d_name, char *dst_name)
{
    assert(d_name != NULL && d_name[0] != 0);
    struct file_info        *pfile_info = NULL;
    assert(dir_info != NULL && stat != NULL);

    if(dir_info->p_filenode == NULL || dir_info->free_size == 0)/*内存不够*/
        realloc_file_info(dir_info);

    pfile_info = dir_info->p_filenode + dir_info->used_size;
    strncpy(pfile_info->fil_name, d_name, MAX_PATH-1);
    memmove(&pfile_info->statbuf, stat, sizeof(struct stat));
    if(dst_name != NULL)     /*d_name是链接文件*/
    {
        pfile_info->is_link = 1;
        strncpy(pfile_info->dst_name, dst_name, MAX_PATH-1);
    }
    else
    {
        pfile_info->is_link = 0;
        pfile_info->dst_name[0] = 0;
    }

    --(dir_info->free_size);
    ++(dir_info->used_size);
}

static void
add_dir(char *pdir, struct list_head *head)
{
    assert(pdir != NULL && pdir[0] != 0 && head != NULL);
    struct dir_list         *pnode = NULL;

    if((pnode = (struct dir_list*)malloc(sizeof(struct dir_list))) == NULL)
    {
        printf("malloc dir_list error\n");
        exit(1);
    }
    memset(pnode, 0, sizeof(struct dir_list));
    strncpy(pnode->dir_path, pdir, MAX_PATH-1);
    list_add_tail(&pnode->list_node, head);
}

/*
    以最后状态修改时间排序函数
*/
static int
cmp_last_statetime(const void *arg1, const void *arg2)
{
    struct file_info   *p1 = (struct file_info*)arg1;
    struct file_info   *p2 = (struct file_info*)arg2;

    return p1->statbuf.st_ctime - p2->statbuf.st_ctime;
}


/*
    以文件最后修改时间排序函数
*/
static int
cmp_last_change(const void *arg1, const void *arg2)
{
    struct file_info   *p1 = (struct file_info*)arg1;
    struct file_info   *p2 = (struct file_info*)arg2;

    return p1->statbuf.st_mtime - p2->statbuf.st_mtime;
}

/*
    以文件最后访问时间排序函数
*/
static int
cmp_last_accesstime(const void *arg1, const void *arg2)
{
    struct file_info   *p1 = (struct file_info*)arg1;
    struct file_info   *p2 = (struct file_info*)arg2;

    return p1->statbuf.st_atime - p2->statbuf.st_atime;
}

/*
    以文件大小排序函数
*/
static int
cmp_filesize(const void *arg1, const void *arg2)
{
    struct file_info   *p1 = (struct file_info*)arg1;
    struct file_info   *p2 = (struct file_info*)arg2;

    return p1->statbuf.st_size - p2->statbuf.st_size;
}

/*
    以文件名排序函数
*/
static int
cmp_filename(const void *arg1, const void *arg2)
{
    struct file_info   *p1 = (struct file_info*)arg1;
    struct file_info   *p2 = (struct file_info*)arg2;

    return strncasecmp(p1->fil_name, p2->fil_name, MAX_PATH);
}


static void
sort(struct dir_info *dir_info)
{
    /*TODO*/
    assert(dir_info != NULL);
    if(g_com_func != NULL)
        qsort(dir_info->p_filenode, dir_info->used_size,
              sizeof(struct file_info), g_com_func);
}

/*
    扫描一个目录下的所有文件
*/
static int
scan_adir(char *dir_name, struct dir_info *dir_info)
{
    assert(dir_name != NULL);
    assert(dir_info != NULL);
    assert(strlen(dir_name) > 0);
    struct stat             statbuf;
    struct dirent           *dirp;
    DIR                     *dp;
    char                    *ptr;   /*指向目录名的最后*/
    char                    filename[MAX_PATH] = {0};

    if(lstat(dir_name, &statbuf) < 0)
    {
        perror("lstat");
        dead_errno(1);
    }

    if(S_ISDIR(statbuf.st_mode) == 0) /* 不是目录 */
    {
        printf("%s is not a directory\n", dir_name);
        assert(0);
    }

    strcpy(filename, dir_name);
    ptr = filename+strlen(dir_name);
    /*使目录始终带有/结束*/
    if(ptr[-1] != '/')
    {
        ptr[0] = '/';
        ++ptr;
    }

    /*保存该目录名*/
    strncpy(dir_info->dir_path, filename, MAX_PATH-1);

    /*下面是目录了*/
    if((dp = opendir(dir_name)) == NULL)
    {
        printf("opendir %s error: %s\n", dir_name, strerror(errno));
        return 1;
    }

    while((dirp = readdir(dp)) != NULL)
    {
        /*不显示所有信息,就跳过隐藏文件(包括. ..)*/
        if(g_param.a == 0 &&
           (strcmp(dirp->d_name, ".") == 0 ||
            strcmp(dirp->d_name, "..") == 0))
            continue;
        /*拼接文件名*/
        strcpy(ptr, dirp->d_name);
        /*获取文件属性*/
        if(lstat(filename, &statbuf) < 0)
        {
            printf("lstat2 file %s error: %s\n", ptr, strerror(errno));
            continue;
        }
        if(S_ISLNK(statbuf.st_mode)) /*链接文件*/
        {
            char            dst_name[MAX_PATH] = {0};
            if(readlink(dirp->d_name, dst_name, MAX_PATH-1) < 0)
            {
                printf("readlink file %s error: %s\n", dirp->d_name, strerror(errno));
                continue;
            }
            save_file_info(dir_info, &statbuf, dirp->d_name, dst_name);
        }
        else
        {
            if(g_param.R && S_ISDIR(statbuf.st_mode))
                add_dir(filename, &g_dir_head);
            save_file_info(dir_info, &statbuf, dirp->d_name, NULL);
        }
    }

    closedir(dp);
    /*对目录进行排序*/
    sort(dir_info);

    return 0;
}

/*
    得到用户选项参数
    FIXME:
        当用户输入的选项有冲突时，还没做错误性检测
*/
static int
get_param(int argc, char *argv[])
{
    char            c;

    /*长选项--(第一项:名字，第二项:是否带参数， 第三项:填0，第四项:短选项名)*/
    /*返回第四个数(如果第三个数为NULL,不然就返回0)*/
    static struct option long_options[] =
    {
        {"help", 0, 0, 'h'},
        {0, 0, 0, 0}
    };
    /*短参数定义-*/
    static const char short_options[] = "alLrRctuSidCnh";

    while(1)
    {

        int option_index = 0;
        c = getopt_long(argc, argv,
                        short_options,long_options,
                        &option_index);
        if(c == -1)
            break;

        switch(c)
        {
            case 'h':               /*长参数(或短参数)返回的，就只显示帮助信息*/
                g_param.h = 1;
                return -1;          /*返回假出错信息，让上层显示帮助信息*/
            case 'a':
                g_param.a = 1;
                break;
            case 'l':
                g_param.l = 1;
                break;
            case 'L':
                g_param.L = 1;
                break;
            case 'r':
                g_param.r = 1;
                break;
            case 'R':
                g_param.R = 1;
                break;
            case 'c':
                g_param.c = 1;
                break;
            case 't':
                g_param.t = 1;
                break;
            case 'u':
                g_param.u = 1;
                break;
            case 'S':
                g_param.S = 1;
                break;
            case 'n':
                g_param.n = 1;
                break;
            case 'i':
                g_param.i = 1;
                break;
            case 'd':
                g_param.d = 1;
                break;
            case 'C':
                g_param.C = 1;
                break;
            case '?':
                printf("unknown param: %s\n", optarg);
                return -1;
            default:
                printf ("?? getopt returned character code %d \n", c);
                return -2;
        }
    }
    /*对选项进行微调*/
    if(g_param.i == 1 ||
       g_param.d == 1 ||
       g_param.c == 1)
        g_param.l = 1;

    /*校验只能用一种排序方式*/
    {
        int         total = 0;
        if(g_param.c == 1)
            ++total;
        if(g_param.t == 1)
            ++total;
        if(g_param.u == 1)
            ++total;
        if(g_param.S == 1)
            ++total;
        if(g_param.n == 1)
            ++total;

        g_com_func = cmp_filename;  /*先置为默认排序方式*/

        if(total > 1)
        {
            printf("the param sort param error,now sort by filename\n");
        }
        else if(total == 1)     /* 仅在此情况下，才有可能排序方式要变 */
        {
            if(g_param.c) g_com_func = cmp_last_statetime;
            else if(g_param.t) g_com_func = cmp_last_change;
            else if(g_param.u) g_com_func = cmp_last_accesstime;
            else if(g_param.S) g_com_func = cmp_filesize;
        }
    }

    /*校验后面是否还有没解析的参数,即为需要查看的文件或者目录*/
    if (optind < argc)
    {
        struct stat             statbuf;
        while(optind < argc)
        {
            if(lstat(argv[optind], &statbuf) < 0)
            {
                printf("lstat file '%s' error: %s\n", argv[optind++], strerror(errno));
                exit(-1);
            }

            if(S_ISDIR(statbuf.st_mode))
                add_dir(argv[optind++], &g_dir_head);
            else if(S_ISLNK(statbuf.st_mode))
            {
                char            dst_name[MAX_PATH] = {0};
                if(readlink(argv[optind], dst_name, MAX_PATH-1) < 0)
                    continue;
                save_file_info(&g_dir_info, &statbuf, argv[optind++], dst_name);
            }
            else
            {
                save_file_info(&g_dir_info, &statbuf, argv[optind++], NULL);
            }
        }
    }
    else
    {
        add_dir("./", &g_dir_head); /*如果不带文件，则默认为当前目录*/
    }

    return 0;
}

static void
init()
{
    memset(&g_dir_info, 0, sizeof(struct dir_info));
    memset(&g_param, 0, sizeof(struct param));
    INIT_LIST_HEAD(&g_dir_head);
}



static void
_print_detail(struct file_info *info)
{
    int             n;
    char            timebuf[128] = {0};
    struct passwd   *ppasswd = NULL;    /*跟用户相关*/
    struct group    *pgroup = NULL;     /*跟用户组相关*/

    /*打印i节点号*/
    if(g_param.i)
        printf("%-10d", (int)(info->statbuf.st_ino));

    /*打印文件类型*/
    if(S_ISREG(info->statbuf.st_mode))
        putchar('-');
    else if(S_ISDIR(info->statbuf.st_mode))
        putchar('d');
    else if(S_ISCHR(info->statbuf.st_mode))
        putchar('c');
    else if(S_ISBLK(info->statbuf.st_mode))
        putchar('b');
    else if(S_ISFIFO(info->statbuf.st_mode))
        putchar('f');
    else if(S_ISLNK(info->statbuf.st_mode))
        putchar('l');
    else if(S_ISSOCK(info->statbuf.st_mode))
        putchar('s');
    else
        putchar('?');

    /*下面移位的方法比通过宏操作，代码要简洁些*/
    for(n = 8; n >= 0; --n)
    {
        if(info->statbuf.st_mode & (1 << n))
        {
            switch(n%3)
            {
                case 2:
                    putchar('r');
                    break;
                case 1:
                    putchar('w');
                    break;
                case 0:
                    putchar('x');
                    break;
            }
        }
        else
            putchar('-');
    }

    putchar(' ');

    /*打印硬连接数*/
    printf("%-2d", info->statbuf.st_nlink);

    /*打印用户ID*/
    if((ppasswd = getpwuid(info->statbuf.st_uid)) != NULL)
        printf("%-8.8s", ppasswd->pw_name);
    else
        printf("%-5d", info->statbuf.st_uid);
    /*打印用户组ID*/
    if((pgroup = getgrgid(info->statbuf.st_gid)) != NULL)
        printf("%-8.8s", pgroup->gr_name);
    else
        printf("%-5d", info->statbuf.st_gid);
    /*打印文件大小*/
    printf("%-10d", (int)(info->statbuf.st_size));

    /*按需打印块大小*/
    if(g_param.d)
        printf("%-6d", (int)(info->statbuf.st_blksize));
    /*打印时间*/
    ctime_r(&(info->statbuf.st_mtime), timebuf);
    if(timebuf[0] != 0)
        timebuf[strlen(timebuf)-1] = 0; /*去掉'\n'*/
    printf("%-20.20s", timebuf);
    /*打印文件名*/
    printf("%s", info->fil_name);

    if(info->is_link && (g_param.L == 0))
        printf(" -> %s", info->dst_name);

    putchar('\n');

}


static void
_printf_data2(struct file_info *pfile)
{
    if(g_param.l)   /*长列表方式*/
    {
        _print_detail(pfile);
    }
    else            /*短列表方式*/
    {
        printf("%s\t", pfile->fil_name);
    }
}
/*仅仅打印一个目录信息*/
static void
_printf_data()
{
    size_t                  i;

    /* 按需要打印目录 */
    if(g_param.l && g_param.R)
        printf("%s\n", g_dir_info.dir_path);

    /*  按需要打印total字段 */
    if(g_param.l && g_dir_info.need_print_total)
        printf("total %d\n", g_dir_info.dir_size);

    if(g_param.r)    /*反排序*/
    {
        for(i = g_dir_info.used_size; i > 0; --i)       /*note i是无符号数，不能用i >= 0来判断*/
            _printf_data2(g_dir_info.p_filenode + i - 1);   /*换成数组下标，要减1*/
    }
    else
    {
        for( i = 0; i < g_dir_info.used_size; ++i)
            _printf_data2(g_dir_info.p_filenode + i);
    }

    /*FIX ME 这里只在短列表在打印换行符，这儿我处理得不大好*/
    if(g_param.l == 0)
        putchar('\n');

    if(g_param.l && g_param.R)
        putchar('\n');
}

/*释放一个目录占有的内存信息，并将目录信息进行初始化*/
static void
_freedir2init()
{
    if(g_dir_info.p_filenode != NULL)
        free(g_dir_info.p_filenode);

    memset(&g_dir_info, 0, sizeof(struct dir_info));;
}

/*打印一个目录，并重新初始化目录信息*/
static void
print_data2free_dir()
{
    if(g_dir_info.used_size == 0)
        return;

    _printf_data();
    _freedir2init();
}

static struct dir_list*
get_adir()
{
    if(list_empty(&g_dir_head))
        return NULL;

    return list_entry(g_dir_head.next, struct dir_list, list_node);
}

/*循环看是否有目录需要遍历*/
static void
recur()
{
    struct dir_list             *pdir;

    while((pdir = get_adir()) != NULL)
    {
        assert(pdir != NULL && pdir->dir_path[0] != 0);
        scan_adir(pdir->dir_path, &g_dir_info);
        cal_dir_size();             /*是完整目录就要计算目录大小了*/
        print_data2free_dir();
        list_del(&(pdir->list_node));
        free(pdir->dir_path);
    }
}

int
main(int argc, char *argv[])
{
    int                 err;

    init();

    if((err = get_param(argc, argv)) < 0)
    {
        printf_usage();
        return err;
    }

    print_data2free_dir();

    recur();

    return 0;
}

