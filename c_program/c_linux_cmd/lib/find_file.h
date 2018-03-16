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
    ����ֵ:
        == 0    �ҵ��ļ����ҵ����ļ���findedname��
        != 0    û�ҵ��ļ�
*/
int
scan_dir(  const char  *dir_name,                               /* ���ĸ�Ŀ¼���в���           */
           const char  *match_str,                               /* ��Ҫ���ҵ��ļ�ƥ���         */
           int (*match_func)(const char *, const char*),         /* ȷ���Ƿ�ƥ��ĺ���           */
           void (*ope_func)(const char *),                       /* �ҵ��ļ��Ĳ���               */
           int         is_findfirst                              /* �Ƿ�ֻ�ҵ�һ��               */
        )
{
    struct stat             statbuf;
    struct dirent           *dirp;
    DIR                     *dp;
    char                    *ptr;   /*ָ��Ŀ¼�������*/
    char                    filename[MAX_PATH] = {0};

    if(lstat(dir_name, &statbuf) < 0)
    {
        //printf("lstat %s error: %s\n", dir_name, strerror(errno));
        return errno;
    }

    if(S_ISDIR(statbuf.st_mode) == 0) /* ����Ŀ¼ */
    {
        printf("%s is not a directory\n", dir_name);
        assert(0);
    }

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
        //printf("opendir %s error: %s\n", dir_name, strerror(errno));
        return errno;
    }

    while((dirp = readdir(dp)) != NULL)
    {
        /*����ʾ������Ϣ,�����������ļ�(����. ..)*/
        if((strcmp(dirp->d_name, ".") == 0 ||
            strcmp(dirp->d_name, "..") == 0))
            continue;
        /*ƴ���ļ���*/
        strcpy(ptr, dirp->d_name);
        /*��ȡ�ļ�����*/
        if(lstat(filename, &statbuf) < 0)
        {
            printf("lstat2 file %s error: %s\n", filename, strerror(errno));
            continue;
        }
        if(S_ISDIR(statbuf.st_mode)) /*Ŀ¼*/
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
