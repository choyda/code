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
    �ı䵥���ļ���mode

    ����ֵ:
        ==  0   �ɹ�
        !=  0   ʧ��(errno)
*/
static int
_chmod_(char *filename,     /*  ��Ҫ�ı���ļ���    */
        mode_t mode             /*  �ı���mode        */
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
    Ŀ¼�����һλ��ִ��Ȩ�ޣ������Ҵ�������ִ��Ȩ�ޣ���ʵ��
*/
static void
change_dir_mode(mode_t *mode )
{
    int         i;

    for(i = 0; i < 3; ++i)
        *mode = (*mode) | (1 << (i*3));
}

/*
    �ݹ�ı��ļ���mode

    ����ֵ:
        ==  0   �ɹ�
        !=  0   ʧ��(errno)
*/
static int
_chmod_r(char *dir_name,     /*  ��Ҫ�ı���ļ���    */
         mode_t mode             /*  �ı���mode        */
        )
{
    struct stat             sta;
    struct dirent           *dirp;
    DIR                     *dp;
    char                    *ptr;   /*ָ��Ŀ¼�������*/
    char                    filename[256] = {0};
    int                     err;
    mode_t                  dir_mode;

    if(lstat(dir_name, &sta) < 0)
        return errno;

    if(S_ISLNK(sta.st_mode))
        return 0;
    else if(S_ISDIR(sta.st_mode) == 0)
        return _chmod_(dir_name, mode);

    /*  ������Ŀ¼��    */
    dir_mode = mode;
    change_dir_mode(&dir_mode);
    if((err = _chmod_(dir_name, dir_mode)) != 0)
        return err;

    strcpy(filename, dir_name);
    ptr = filename+strlen(dir_name);
    /*ʹĿ¼ʼ�մ���/����*/
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

        /*ƴ���ļ���*/
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
    �ı��ļ���mode

    ����ֵ:
        ==  0   �ɹ�
        !=  0   ʧ��(errno)
*/
int
_chmod(char *filename,  /*  ��Ҫ�ı���ļ���    */
       mode_t mode,        /*  �ı���mode        */
       int is_r            /*  �Ƿ�ݹ�            */
      )
{
    if(is_r)
        return _chmod_r(filename, mode);
    else
        return _chmod_(filename, mode);
}


#endif
