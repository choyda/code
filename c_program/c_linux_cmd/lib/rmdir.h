#ifndef _RM_DIR_H_
#define _RM_DIR_H_

#include <errno.h>
#include <unistd.h>

/*
    ɾ��һ����Ŀ¼
    ����ֵ:
        ==  0   �ɹ�
        >   0   ϵͳ�������
        <   0   �û��������
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

