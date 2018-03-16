/*
    ����ļ���������
*/
#ifndef _RENAME_H_
#define _RENAME_H_


#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "file_isexist.h"


/*
    ����ֵ:
    ==0     �ɹ�
    !=0     ʧ��    (��Ӧerrno)
*/
int
_rename(char *oldname,  /*   ��Ҫ��������Դ�ļ���         */
        char *newname              /*   ����������ļ���           */
       )
{
    assert(oldname != NULL && oldname[0] != 0);
    assert(newname != NULL && newname[0] != 0);

    if(file_isexist(newname))
    {
        fprintf(stderr,
                "dstfile [%s] is existed\n",
                newname);

        return EEXIST;
    }

    if(rename(oldname, newname) != 0)
    {
        fprintf(stderr,
                "rename [%s] to [%s] error: [%s]\n",
                oldname, newname, strerror(errno));

        return errno;
    }

    return 0;
}

#endif

