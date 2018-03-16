#ifndef _FIND_H_
#define _FIND_H_

#include <assert.h>
#include <errno.h>
#include "./str_match.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#ifndef MAX_PATH
#define MAX_PATH        256
#endif

int
is_excute(const char *path)
{
    return !access(path, X_OK);
}

int
which_matched(   const char *filename,
                 const char *match)
{
    char *base_name = strrchr(filename, '/');
    
    if(base_name == NULL)
        return 0;
    base_name++;
    return (strcmp(base_name, match) == 0 && 
            is_matched(match, filename)&&
            is_excute(filename));
}

void
which_ope(const char *filename)
{
    printf("\t%s\n", filename);
}

int
find_matched(   const char *filename,
                       const char *match)
{
    return is_matched(match, filename);
}

void
find_ope(const char *filename)
{
    printf("%s\n", filename);
}

/*
    返回值:
        == 0    找到文件，找到的文件在findedname中
        != 0    没找到文件
*/
int
scan_dir(  const char  *dir_name,                               /* 在哪个目录进行查找           */
           const char  *match_str,                               /* 需要查找的文件匹配符         */
           int (*match_func)(const char *, const char*),         /* 确定是否匹配的函数           */
           void (*ope_func)(const char *),                       /* 找到文件的操作               */
           int         is_findfirst                              /* 是否只找第一个               */
        )
{
    struct stat             statbuf;
    struct dirent           *dirp;
    DIR                     *dp;
    char                    *ptr;   /*指向目录名的最后*/
    char                    filename[MAX_PATH] = {0};

    if(lstat(dir_name, &statbuf) < 0)
    {
        //printf("lstat %s error: %s\n", dir_name, strerror(errno));
        return errno;
    }

    if(S_ISDIR(statbuf.st_mode) == 0) /* 不是目录 */
    {
        printf("%s is not a directory\n", dir_name);
        assert(0);
    }

    strcpy(filename, dir_name);
    ptr = filename+strlen(dir_name);
    /*使目录始终带有/结束*/
    if(ptr[-1] != '/')
    {
        ptr[0] = '/';
        ++ptr;
    }

    /*下面是目录了*/
    if((dp = opendir(dir_name)) == NULL)
    {
        //printf("opendir %s error: %s\n", dir_name, strerror(errno));
        return errno;
    }

    while((dirp = readdir(dp)) != NULL)
    {
        /*不显示所有信息,就跳过隐藏文件(包括. ..)*/
        if((strcmp(dirp->d_name, ".") == 0 ||
            strcmp(dirp->d_name, "..") == 0))
            continue;
        /*拼接文件名*/
        strcpy(ptr, dirp->d_name);
        /*获取文件属性*/
        if(lstat(filename, &statbuf) < 0)
        {
            printf("lstat2 file %s error: %s\n", filename, strerror(errno));
            continue;
        }
        if(S_ISDIR(statbuf.st_mode)) /*目录*/
        {
            scan_dir(filename, match_str, match_func, ope_func, is_findfirst);
            continue;
        }

        if(match_func && match_func(filename, match_str) && ope_func)
        {
            ope_func(filename);
            if(is_findfirst)
                break;
        }
    }

    closedir(dp);

    return 0;
}

#endif
