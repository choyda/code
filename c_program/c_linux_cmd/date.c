/*
    date.c
    功能:
        模拟錿date命令
    历史记录:
        作者        时间        操作        联系方式
        卢富波      2014-05-13  创建        1164830775@qq.com
*/

#include "lib/command_comm.h"
#include "lib/gettime.h"
#include "lib/is_digit.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>


static void
date_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s  [OPTION]\n", program_name);
        printf("-h  --help(--h)                         show the help infomation\n");
        printf("--date=[YEAR|MON|DAY|HOUR|MIN|SEC(a 14bit number)]      set the system time\n");
    }

    exit(status);
}

/*
    将字符串时间转换为tm类型

    返回值:
        == 0    成功
        != 0    失败

   FIXME
        字符串时间仅支持类似20140516113559(2014年5月16日11点35分59秒)
*/
static int
time_char2tm(char * ptime,  struct tm *t)
{
#define     MY_YEAR     4
#define     MY_MON      2
#define     MY_DAY      2
#define     MY_HOUR     2
#define     MY_MIC      2
#define     MY_SEC      2

    const int   time_size = 14; /*   应该传入的时间字符串长度   */
    char        tmp[8];
    size_t      off_size = 0;
    int         tmp_n;

    if(ptime == NULL || t == NULL)
        return -10;

    if(strlen(ptime) != time_size)
    {
        fprintf(stderr,
                "must a 14 bit date, for example(20140202112233)\n");
        return -1;
    }

    memset(t, 0, sizeof(struct tm));

    if(is_digit(ptime, strlen(ptime)) == 0)
        return -2;

    /*  取年    */
    strncpy(tmp, ptime + off_size, MY_YEAR);
    tmp[MY_YEAR] = 0;
    tmp_n = atoi(tmp);
    if(tmp_n < 1900)
        return -3;
    t->tm_year = tmp_n - 1900;
    off_size += MY_YEAR;

    /*  取月    */
    strncpy(tmp, ptime + off_size, MY_MON);
    tmp[MY_MON] = 0;
    tmp_n = atoi(tmp);
    if(tmp_n < 1 || tmp_n > 12)
        return -4;
    t->tm_mon = tmp_n - 1;
    off_size += MY_MON;

    /*  取日    */
    strncpy(tmp, ptime + off_size, MY_DAY);
    tmp[MY_DAY] = 0;
    tmp_n = atoi(tmp);
    if(tmp_n < 1 || tmp_n > 31)
        return -5;
    t->tm_mday = tmp_n;
    off_size += MY_DAY;

    /*  取时    */
    strncpy(tmp, ptime + off_size, MY_HOUR);
    tmp[MY_HOUR] = 0;
    tmp_n = atoi(tmp);
    if(tmp_n < 0 || tmp_n > 23)
        return -6;
    t->tm_hour = tmp_n;
    off_size += MY_HOUR;

    /*  取分    */
    strncpy(tmp, ptime + off_size, MY_MIC);
    tmp[MY_MIC] = 0;
    tmp_n = atoi(tmp);
    if(tmp_n < 0 || tmp_n > 59)
        return -7;
    t->tm_min = tmp_n;
    off_size += MY_MIC;

    /*  取秒    */
    strncpy(tmp, ptime + off_size, MY_SEC);
    tmp[MY_SEC] = 0;
    tmp_n = atoi(tmp);
    if(tmp_n < 0 || tmp_n > 60)
        return -8;
    t->tm_sec = tmp_n;
    off_size += MY_SEC;

#undef     MY_YEAR
#undef     MY_MON
#undef     MY_DAY
#undef     MY_HOUR
#undef     MY_MIC
#undef     MY_SEC

    return 0;
}

static int
pr_sys_time()
{
    struct timeval          time_spec;
    //struct tm               *t;
    int                     err;

    if((err = get_sys_time(&time_spec)) != 0)
        return -1;

    printf("%s", ctime(&time_spec.tv_sec));

    return 0;
}

static int
set_sys_time(char *new_date)
{
    struct tm           t_tm;
    struct timeval      t_timeval;
    int                 err;

    if((err = time_char2tm(new_date, &t_tm)) != 0)
    {
        fprintf(stderr,
                "[%s] is not a correct time\n",
                new_date);
        return err;
    }

    t_timeval.tv_sec = mktime(&t_tm);
    t_timeval.tv_usec = 0;

    if((err = settimeofday(&t_timeval, 0)) != 0)
    {
        if(errno == EPERM)
        {
            fprintf(stderr,
                    "[%s] is seted by root user\n",
                    program_name);

            return 0;
        }
        return -11;
    }

    return 0;
}
int
main(int argc, char *argv[])
{
    int                     c;
    int                     is_set = 0;     /*  是否是设置时间标志    */
    char                    *new_date = NULL;

    static struct option const long_options[] =
    {
        {"help", 0, NULL, 'h'},
        {"date", 1, NULL, 'd'},
        {NULL, 0, NULL, 0}
    };
    exit_status = 0;
    program_name = argv[0];
    while((c = getopt_long(argc, argv,
                           "hH", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                date_usage_exit(EXIT_SUCESS);
                break;
            case 'd':
                if (optarg)
                {
                    new_date=optarg;
                    is_set = 1;
                }
                break;
            default:
                date_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    if(optind != argc)
        date_usage_exit(1);

    if(is_set && new_date != NULL)
        exit_status = set_sys_time(new_date);
    else
        exit_status = pr_sys_time();

    if(exit_status != 0)
        fprintf(stderr,
                "[%s] error with errno[%d]\n",
                program_name, exit_status);

    exit(exit_status);
}

#if 0
int
main(int argc, char *argv[])
{

    struct timeval          time_spec;
    //struct tm               *t;
    int                     err;

    if((err = get_sys_time(&time_spec)) != 0)
        return -1;

    t = localtime(&time_spec.tv_sec);
    if(t == NULL)
        return -2;


    int                     err;
    struct tm               t;

    err = time_char2tm(argv[1], &t);
    if(err != 0)
    {
        printf("[%s] error[%d]\n", argv[1], err);

        return -3;
    }



    printf("%s", asctime(&t));

    printf("%s", ctime(&ti));

    return 0;
}

#endif
