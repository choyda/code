#ifndef _GET_FILE_SIZE_H_
#define _GET_FILE_SIZE_H_

#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifndef MAX_PATH
#define MAX_PATH        256
#endif

/*�õ�ĳһ���ļ��Ĵ�С*/
static int
get_filesize(const char *filename)
{
    struct stat         buf;

    if(filename == NULL || filename[0] == 0)
        return -1;

    if(lstat(filename, &buf) != 0)
        return -2;

    if(S_ISDIR(buf.st_mode))
        return -3;
    
    return buf.st_size;
}

/*�õ�ĳһ��Ŀ¼�������ļ���ĳ���ļ��Ĵ�С
Ŀ¼���������ļ��Ĵ�С֮��*/
int
get_dirsize(const char *dir_name)
{
    struct stat             statbuf;
    struct dirent           *dirp;
    DIR                     *dp;
    char                    *ptr;   /*ָ��Ŀ¼�������*/
    char                    filename[MAX_PATH] = {0};
    int                     dir_size = 0;
    int                     err;

    if(dir_name == NULL || dir_name[0] == 0)
        return -1;

    if(lstat(dir_name, &statbuf) != 0)
        return -2;

    if(!S_ISDIR(statbuf.st_mode))
        return get_filesize(dir_name);

    strcpy(filename, dir_name);
    ptr = filename+strlen(dir_name);
    /*ʹĿ¼ʼ�մ���/����*/
    if(ptr[-1] != '/')
    {
        ptr[0] = '/';
        ++ptr;
    }
    
    /*������Ŀ¼��*/
    if((dp = opendir(dir_name)) == NULL)
    {
        printf("opendir %s error: %s\n", dir_name, strerror(errno));
        return -3;
    }

    while((dirp = readdir(dp)) != NULL)
    {
        if(strcmp(dirp->d_name, ".") == 0||
           strcmp(dirp->d_name, "..")== 0)
            continue;

        /*ƴ���ļ���*/
        strcpy(ptr, dirp->d_name);
        /*��ȡ�ļ�����*/
        if(lstat(filename, &statbuf) < 0)
        {
            printf("lstat2 file %s error: %s\n", filename, strerror(errno));
            return -4;
        }
        if(S_ISDIR(statbuf.st_mode)) /*Ŀ¼*/
        {
            if((err = get_dirsize(filename)) < 0)
                return -5;
            dir_size += err;
        }
        else
        {
            dir_size += statbuf.st_size;
            printf("%ld\t%s\n", statbuf.st_size, filename);
        }
    }

    closedir(dp);
    printf("%d\t%s\n", dir_size, dir_name);
    return dir_size;
}

#endif

