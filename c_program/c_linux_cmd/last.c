/*
    last.c
    功能:
        模拟last命令
    历史记录:
        作者        时间        操作        联系方式
        卢富波      2014-06-04  创建        1164830775@qq.com
*/

#include "lib/command_comm.h"
#include "lib/list.h"
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <paths.h>
#include <sys/time.h>
#include <sys/types.h>
#include <bits/wordsize.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <utmp.h>

#define UTMP_PATH   "/var/log/wtmp"

struct ttylist
{
    char                tty[UT_LINESIZE + 1];
    struct list_head    list_node;
};


/* The structure describing the status of a terminated process.  This
   type is used in `struct utmp' below.  */
struct status
{
    short int e_termination;    /* Process termination status.  */
    short int e_exit;       /* Process exit status.  */
};


struct list_head        g_head;

static void
last_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s  \n", program_name);
        printf("-h(-H)  --help  show help information\n");
    }

    exit(status);
}

static int
is_existed(char *device)
{
    struct list_head    *tmp;
    struct ttylist      *pnode;

    list_for_each(tmp, &g_head)
    {
        pnode = list_entry(tmp, struct ttylist, list_node);
        if(strncmp(device, pnode->tty, sizeof(pnode->tty)) == 0)
            return 1;
    }

    return 0;
}

static int
insert(char *device)
{
    if(is_existed(device))
        return 1;

    struct ttylist      *pnode = (struct ttylist*)malloc(sizeof(struct ttylist));
    if(pnode == NULL)
        exit(1);

    memset(pnode, 0, sizeof(struct ttylist));
    strncpy(pnode->tty, device, sizeof(pnode->tty));

    list_add_tail(&(pnode->list_node), &g_head);

    return 0;
}

static void
destroy_list()
{
    struct list_head    *tmp, *psafe;
    struct ttylist      *pnode;

    list_for_each_safe(tmp, psafe, &g_head)
    {
        pnode = list_entry(tmp, struct ttylist, list_node);
        list_del(&(pnode->list_node));
        free(pnode);
    }
}

static void
print_record(struct utmp *record)
{
    if(record == NULL || record->ut_user[0] == 0)
        return;

    char    logininfo[128];
    char    timebuf[128] = {0};

    if(insert(record->ut_line) == 0)
        strcpy(logininfo, "still login");
    else
        strcpy(logininfo, "had logout");

    ctime_r(&(record->ut_tv.tv_sec), timebuf);
    if(timebuf[0] != 0)
        timebuf[strlen(timebuf)-1] = 0; /*去掉'\n'*/

    printf("%-15s%-15s%-20s%-30s%-15s\n",
           record->ut_user,
           record->ut_line,
           record->ut_host,
           timebuf,
           logininfo
          );
}

static int
last()
{
    struct stat         st;
    struct utmp         *utl, *cur_record;
    int                 fd, listnr, i;

    if((fd = open(UTMP_PATH, O_RDONLY)) < 0)
    {
        fprintf(stderr, "open file '%s' error: %s\n", UTMP_PATH, strerror(errno));
        return errno;
    }

    fstat(fd, &st);
    listnr = st.st_size/sizeof(struct utmp);
    utl = mmap(NULL, st.st_size, PROT_READ|PROT_WRITE,
               MAP_PRIVATE|MAP_FILE, fd, 0);
    if (utl == NULL)
    {
        fprintf(stderr, "mmap error: %s\n", strerror(errno));
        return errno;
    }

    for(i = listnr-1 ; i >= 0; --i)
    {
        cur_record = utl + i;
        print_record(cur_record);
    }

    munmap(utl,st.st_size);
    close(fd);

    return 0;
}


int
main(int argc, char *argv[])
{
    int                 c;

    static struct option const long_options[] =
    {
        {"help", 0, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };

    exit_status = 0;
    program_name = argv[0];
    while((c = getopt_long(argc, argv,
                           "Hs", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                last_usage_exit(EXIT_SUCESS);
                break;
            default:
                last_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    if(argc - optind != 0)
        last_usage_exit(EXIT_FAILURE);

    INIT_LIST_HEAD(&g_head);
    exit_status = last();
    destroy_list();

    exit(exit_status);
}

