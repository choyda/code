#ifndef _GET_TIME_H_
#define _GET_TIME_H_

#include <sys/time.h>

/*
    �õ���ǰ��ϵͳʱ��
    ����ֵ
        == 0    �ɹ�
        != 0    ʧ��
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

