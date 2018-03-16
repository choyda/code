#ifndef _REMOVE_H_
#define _REMOVE_H_


#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include "yesno.h"
#include "file_isexist.h"

struct rm_option
{
    int             r;      /* 递归删除         */
    int             f;      /* 强制删除         */
};


int
rm_simple(const char *file, const struct rm_option *option)
{
    if(option->f == 0)
    {
        printf("rm '%s'?", file);
        if(yesno() == 0)
            return 0;
    }

    if(unlink(file) != 0)
    {
        fprintf(stderr, "unlink file '%s' error: %s\n", file, strerror(errno));
        return errno;
    }

    return 0;
}


int
rm_dir(const char *file_pre, const struct rm_option *option)
{
    struct stat             statbuf;
    struct dirent           *dirp;
    DIR                     *dp;
    int                     err = 0;
    char                    file[256] = {0};
    char                    tmpfile[256] = {0};

    strncpy(file, file_pre, sizeof(file));

    if(lstat(file, &statbuf) < 0)
    {
        fprintf(stderr, "lstat [%s] error[%s]\n",
                file, strerror(errno));
        return errno;
    }

    if(S_ISDIR(statbuf.st_mode) == 0)   /*  不是目录    */
    {
        return rm_simple(file, option);
    }

    /*  是目录  */
    if(strlen(file) > 0 && file[strlen(file) - 1] != '/')
    {
        file[strlen(file)] = '/';
        file[strlen(file) + 1] = 0;
    }

    strcpy(tmpfile, file);
    if((dp = opendir(file)) == NULL)
    {
        fprintf(stderr, "opendir [%s] error [%s]\n",
                file, strerror(errno));
        return errno;
    }

    while((dirp = readdir(dp)) != NULL)
    {
        if(strcmp(dirp->d_name, ".") == 0 ||
           strcmp(dirp->d_name, "..") == 0)
            continue;
        memset(tmpfile, 0, sizeof(tmpfile));
        strcpy(tmpfile, file);
        strcat(tmpfile, dirp->d_name); /*  拼接文件名    */
        err |= rm_dir(tmpfile, option);
    }

    closedir(dp);

    if(rmdir(file) != 0)
    {
        fprintf(stderr, "rmdir [%s] error [%s]\n",
                file, strerror(errno));
        return errno;
    }

    return err;
}



/*
    成功:
        0
    失败
        != 0
*/
int
rm( char *file, const struct rm_option *option)
{
    assert(file != NULL && file[0] != 0);
    assert(option != NULL);

    if(!file_isexist(file))
    {
        fprintf(stderr, "file '%s' does not exist\n", file);
        return 1;
    }

    if(option->r)
        return rm_dir(file, option);
    else
        return rm_simple(file, option);
}


#endif
