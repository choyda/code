#ifndef _MK_DIR_H_
#define _MK_DIR_H_

#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include "./file_isexist.h"

/*
    创建单层目录

    返回值:
    ==  0   成功
    >   0   操作系统的失败(errno返回)
    <   0   用户层的失败

    FIXME:
    mode_t的注意事项由外层控制
*/
int
_mkdir(const char *path,      /*  需要创建目录的目录名    */
       const mode_t mode        /*  目录的mode_t类型        */
      )
{
    int             save_errno, ret;

    if(path == NULL ||
       path[0] == 0)
        return -1;
    save_errno = errno;
    if(mkdir(path, mode) != 0)
    {
        ret = errno;
        errno = save_errno;
        fprintf(stderr,
                "mkdir [%s] error [%s]\n",
                path, strerror(ret));

        return ret;
    }

    return 0;
}

/*
    创建多层目录(包括单层目录)

    返回值:
    ==  0   成功
    >   0   操作系统的失败(errno返回)
    <   0   用户层的失败

    FIXME:
    mode_t的注意事项由外层控制
*/

int
_mkdir_p(const char *path,   /*  需要创建目录的目录名    */
         const mode_t mode        /*  目录的mode_t类型        */
        )
{
    char                localpath[256];
    char                tmpdir[256];
    const char          *seps = "/";    /*  目录的分隔符    */
    char                *pstrstr;
    int                 ret = 0;
    int                 path_size = 0;

    if(path == NULL ||
       path[0] == 0)
        return 0;

    snprintf(localpath, sizeof(localpath), "%s", path);
    path_size = strlen(localpath);
    if(path_size > 0 &&
       localpath[path_size - 1] != '/' &&
       path_size < sizeof(localpath) - 2)
    {
        localpath[path_size] = '/';
        localpath[path_size + 1] = 0;

    }

    pstrstr = strstr(localpath, seps);
    while(pstrstr != NULL)
    {
        if(pstrstr != localpath)
        {
            snprintf(tmpdir, pstrstr-localpath+1, "%s", localpath);
            if(file_isexist(tmpdir) == 0)
            {
                ret |= _mkdir(tmpdir, mode);
                if(ret != 0) return ret;
            }
            else
            {
                if(file_isdir(tmpdir) == 0)
                {
                    printf("[%s] is existed but is not a directory\n", tmpdir);
                    return 17;
                }
            }
        }
        pstrstr = strstr(pstrstr+1, seps);
    }

    return ret;
}

#endif

