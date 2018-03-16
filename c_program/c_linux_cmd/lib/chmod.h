#ifndef _CHMOD_H_
#define _CHMOD_H_
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/*
    改变单个文件的mode

    返回值:
        ==  0   成功
        !=  0   失败(errno)
*/
static int
_chmod_(char *filename,     /*  需要改变的文件名    */
        mode_t mode             /*  改变后的mode        */
       )
{
    int         ret = 0;
    int         save_errno = errno;

    if(chmod(filename, mode) != 0)
    {
        ret = errno;
        errno = save_errno;
    }

    return ret;
}

/*
    目录必须带一位可执行权限，这里我带三个可执行权限，简单实现
*/
static void
change_dir_mode(mode_t *mode )
{
    int         i;

    for(i = 0; i < 3; ++i)
        *mode = (*mode) | (1 << (i*3));
}

/*
    递归改变文件的mode

    返回值:
        ==  0   成功
        !=  0   失败(errno)
*/
static int
_chmod_r(char *dir_name,     /*  需要改变的文件名    */
         mode_t mode             /*  改变后的mode        */
        )
{
    struct stat             sta;
    struct dirent           *dirp;
    DIR                     *dp;
    char                    *ptr;   /*指向目录名的最后*/
    char                    filename[256] = {0};
    int                     err;
    mode_t                  dir_mode;

    if(lstat(dir_name, &sta) < 0)
        return errno;

    if(S_ISLNK(sta.st_mode))
        return 0;
    else if(S_ISDIR(sta.st_mode) == 0)
        return _chmod_(dir_name, mode);

    /*  下面是目录了    */
    dir_mode = mode;
    change_dir_mode(&dir_mode);
    if((err = _chmod_(dir_name, dir_mode)) != 0)
        return err;

    strcpy(filename, dir_name);
    ptr = filename+strlen(dir_name);
    /*使目录始终带有/结束*/
    if(ptr[-1] != '/')
    {
        ptr[0] = '/';
        ++ptr;
    }

    if((dp = opendir(dir_name)) == NULL)
        return errno;

    while((dirp = readdir(dp)) != NULL)
    {
        if(strcmp(dirp->d_name, ".") == 0 ||
           strcmp(dirp->d_name, "..") == 0)
            continue;

        /*拼接文件名*/
        strcpy(ptr, dirp->d_name);
        if(lstat(filename, &sta) < 0)
            return errno;

        if(S_ISLNK(sta.st_mode))
            continue;
        else if(S_ISDIR(sta.st_mode))
        {
            if((err = _chmod_r(filename, mode)) != 0)
                return err;
        }
        else
        {
            if((err = _chmod_(filename, mode)) != 0)
                return err;
        }
    }

    closedir(dp);

    return 0;
}


/*
    改变文件的mode

    返回值:
        ==  0   成功
        !=  0   失败(errno)
*/
int
_chmod(char *filename,  /*  需要改变的文件名    */
       mode_t mode,        /*  改变后的mode        */
       int is_r            /*  是否递归            */
      )
{
    if(is_r)
        return _chmod_r(filename, mode);
    else
        return _chmod_(filename, mode);
}


#endif
