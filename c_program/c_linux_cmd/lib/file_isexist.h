/*
    ����һ���ļ��Ƿ����
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
    ��������:
        �ж��ļ�(Ŀ¼)�Ƿ����
    ����ֵ:
        ==  0   ������
        ==  1   ����
*/
int 
file_isexist(const char *filename /*��Ҫ�����Ƿ���ڵ��ļ���*/ )
{
    assert(filename != NULL && filename[0] != 0);

    if(access(filename, R_OK) < 0)
        return 0;

    return 1;
}

/*
    ��������:
        �ж��ļ��Ƿ����,��������ͨ�ļ�
    ����ֵ:
        ==  0   ������
        ==  1   ����
*/
int 
file_isreg(const char *filename /*��Ҫ�����Ƿ���ڵ��ļ���*/ )
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
    ��������:
        �ж��ļ��Ƿ����,������Ŀ¼
    ����ֵ:
        ==  0   ������
        ==  1   ����
*/
int 
file_isdir(const char *filename /*��Ҫ�����Ƿ���ڵ��ļ���*/ )
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
    �õ��ļ���С
    ����ֵ:
    <   0     ����
    >=  0     �ļ���С
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

