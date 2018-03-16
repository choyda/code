#ifndef _RM_DIR_H_
#define _RM_DIR_H_

#include <errno.h>
#include <unistd.h>

/*
    删除一个空目录
    返回值:
        ==  0   成功
        >   0   系统层错误码
        <   0   用户层错误码
*/
int
_rmdir(char *dir)
{
    int          save_errno, ret = 0;

    if(dir == NULL || dir[0] == 0)
        return -1;

    save_errno = errno;
    if(rmdir(dir) != 0)
    {
        ret = errno;
        errno = save_errno;
    }

    return ret;
}

#endif

