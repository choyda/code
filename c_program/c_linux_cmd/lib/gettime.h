#ifndef _GET_TIME_H_
#define _GET_TIME_H_

#include <sys/time.h>

/*
    得到当前的系统时间
    返回值
        == 0    成功
        != 0    失败
*/
int
get_sys_time (struct timeval *ts)
{
    struct timeval tv;
    int r = gettimeofday (&tv, 0);
    if (r == 0)
    {
        ts->tv_sec = tv.tv_sec;
        ts->tv_usec = tv.tv_usec * 1000;
    }
    return r;
}

#endif

