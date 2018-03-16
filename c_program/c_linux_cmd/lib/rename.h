/*
    完成文件的重命名
*/
#ifndef _RENAME_H_
#define _RENAME_H_


#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "file_isexist.h"


/*
    返回值:
    ==0     成功
    !=0     失败    (对应errno)
*/
int
_rename(char *oldname,  /*   需要重命名的源文件名         */
        char *newname              /*   重命名后的文件名           */
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

