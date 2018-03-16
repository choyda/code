/*
    测试一个文件是否存在
*/
#ifndef _COMMAND_FILE_ISEXIST_H_
#define _COMMAND_FILE_ISEXIST_H_

#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

/*
    函数功能:
        判断文件(目录)是否存在
    返回值:
        ==  0   不存在
        ==  1   存在
*/
int 
file_isexist(const char *filename /*需要测试是否存在的文件名*/ )
{
    assert(filename != NULL && filename[0] != 0);

    if(access(filename, R_OK) < 0)
        return 0;

    return 1;
}

/*
    函数功能:
        判断文件是否存在,并且是普通文件
    返回值:
        ==  0   不存在
        ==  1   存在
*/
int 
file_isreg(const char *filename /*需要测试是否存在的文件名*/ )
{
    assert(filename != NULL && filename[0] != 0);
    struct stat             statbuf;
    
    if(access(filename, R_OK) < 0)
        return 0;

    if(lstat(filename, &statbuf) == -1)
        return 0;

    if(S_ISREG(statbuf.st_mode))
        return 1;

    return 0;
}

/*
    函数功能:
        判断文件是否存在,并且是目录
    返回值:
        ==  0   不存在
        ==  1   存在
*/
int 
file_isdir(const char *filename /*需要测试是否存在的文件名*/ )
{
    assert(filename != NULL && filename[0] != 0);
    struct stat             statbuf;
    
    if(access(filename, R_OK) < 0)
        return 0;

    if(lstat(filename, &statbuf) == -1)
        return 0;

    if(S_ISDIR(statbuf.st_mode))
        return 1;

    return 0;
}

/*
    得到文件大小
    返回值:
    <   0     出错
    >=  0     文件大小
*/
int
get_file_size(int fd)
{
    struct stat         sta;

    if(fstat(fd, &sta) < 0){
        fprintf(stderr, 
            "get file size error [%s]\n",
            strerror(errno));
        return -1;
    }

    return sta.st_size;
}

#endif

