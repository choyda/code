/*
    touch.c
    功能:
        模拟touch命令:创建空文件或者修改文件时间
    历史记录:
        作者        时间        操作        联系方式
        卢富波      2014-05-15  创建        1164830775@qq.com
*/
#include "lib/command_comm.h"
#include "lib/file_isexist.h"
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <utime.h>
#include <time.h>

struct touch_opt
{
    char            a;      /*  修改访问时间,默认修改选项               */
    char            m;      /*  修改修改时间                            */
    char            c;      /*  文件不存在时不要创建文件,默认是要创建的 */
};

struct touch_opt        g_opt;

void
touch_usage_exit(int status)
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
        printf("-a  --access    modify access time \n");
        printf("-m  --modify    modify modify time\n");
        printf("-c  --no-create if file is not existed do't create file, otherwith, create it\n");
    }

    exit(status);
}


/*
    FIXME
    该函数是，仅文件不存在才会创建的
*/
static int
create_file(char *file)
{
    int                 fd;
#define FILE_MODE S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH
    if((fd = open(file, O_CREAT, FILE_MODE)) < 0)
        return errno;
    else
        close(fd);

    return 0;
}

static int
chg_file_time(char *file, struct utimbuf *t)
{
    if(utime(file, t) != 0)
        return errno;

    return 0;
}

static int
init_time(char *file, struct utimbuf *t)
{
    struct stat     sta;

    if(stat(file, &sta) != 0)
        return errno;

    t->actime = sta.st_atime;
    t->modtime = sta.st_mtime;

    return 0;
}

static int
touch(char *file)
{
    int             err;
    struct utimbuf  chg_time;     /*  需要修改的时间值    */

    if(g_opt.c)
        create_file(file);

    if(file_isexist(file) == 0)
    {
        fprintf(stderr,
                "file [%s] does not existed\n",
                file);

        return -1;
    }


    /*先初始化时间*/
    if((err = init_time(file, &chg_time)) != 0)
        return err;

    /*然后来修改时间了*/
    if(g_opt.a)
        chg_time.actime = time(NULL);
    if(g_opt.m)
        chg_time.modtime = time(NULL);

    return chg_file_time(file, &chg_time);
}

static void
init_opt()
{
    g_opt.a = 0;        /*  不带参数时就是修改访问时间  */
    g_opt.c = 1;        /*  默认要创建文件              */
    g_opt.m = 0;
}

int
main(int argc, char *argv[])
{
    int                     c;

    static struct option const long_options[] =
    {
        {"help", 0, NULL, 'h'},
        {"access", 0, NULL, 'a'},
        {"modify", 0, NULL, 'm'},
        {"no-create", 0, NULL, 'c'},
        {"date", 1, NULL, 'd'},
        {NULL, 0, NULL, 0}
    };
    program_name = argv[0];
    exit_status = 0;
    init_opt();
    while((c = getopt_long(argc, argv,
                           "hamcd:", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                touch_usage_exit(EXIT_SUCESS);
                break;
            case 'a':
                g_opt.a = 1;
                break;
            case 'm':
                g_opt.m = 1;
                break;
            case 'c':
                g_opt.c = 0;    /*  默认是要创建的    */
                break;
            default:
                touch_usage_exit(EXIT_FAILURE);
                break;
        }
    }
    if(optind == argc)
        touch_usage_exit(1);

    while(optind < argc)
        exit_status |= touch(argv[optind++]);

    if(exit_status != 0)
        fprintf(stderr,
                "[%s] some error happed with errno[%d]\n",
                program_name, exit_status);

    exit(exit_status);
}


