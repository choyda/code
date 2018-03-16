/*
    复制文件
*/

#ifndef _COMMAND_COPY_H_
#define _COMMAND_COPY_H_


#include <dirent.h>
#include "./command_comm.h"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <string.h>
#include "file_isexist.h"
#include "yesno.h"
#include "remove.h"


struct cp2mv_option
{
    char                r;      /* 是否递归                                         */
    char                l;      /* 是否是复制链接文件本身，而非实现                 */
    char                f;      /* 当目标文件存在，不提示直接覆盖，默认是要提示的   */
};

#define MAX_NAME_LEN        256

int copy(const char *src_file,        /* 指向需要复制的源文件                 */
         const char *dst_file,          /* 指向复制文件的目标文件               */
         int  is_force,                 /* 当目标文件存在时，是否是强制覆盖     */
         int  deal_link,                /* 是否要复制链接文件                   */
         int  need_del                  /* 是否删除源文件                       */
        );

void
cp2mv_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help -h' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s [OPTION]... SOURCE DEST\n ", program_name);
        printf("%s [OPTION]... SOURCEDIR DESTDIR\n ", program_name);
        printf("DESCRIPTION\n");
        printf("treat all the file as normal fils\n");
        printf("OPTION\n");
        printf("-h, -H  --help          Help infomation\n");
        printf("-r, -R, --recursive     Recursive cp files\n");
        printf("-l, -L, --link          Copy linkfile not the actual file\n");
        printf("-f, -F, --fource        Force to overwrite file without prompt\n");
    }

    exit(status);
}


void
cp_file( char *psrc,  char *pdst, struct cp2mv_option *option, int needdel)
{
    char        dst[MAX_NAME_LEN];
    size_t      name_size;

    printf("cp [%s] [%s]\n", psrc, pdst);

    strncpy(dst, pdst, MAX_NAME_LEN);
    dst[MAX_NAME_LEN - 1] = 0;

    if(file_isdir(psrc))
    {
        fprintf(stderr, "file '%s' is directory\n", psrc);
        return;
    }

    if(file_isdir(pdst))
    {
        name_size = strlen(dst);
        if(name_size > 0 && dst[name_size - 1] != '/' && name_size < MAX_NAME_LEN - 3 )
        {
            dst[name_size++] = '/';
            dst[name_size++] = 0;
        }
        strncpy(dst+name_size, psrc, MAX_NAME_LEN - name_size);
    }

    copy(psrc, dst, option->f, option->l, needdel);
}



void
cp_r( char *psrc,  char *pdst, struct cp2mv_option *option, int need_del)
{
    size_t          name_size;
    struct dirent   *dirp;
    DIR             *dp;
    char            src[256] = {0};
    char            dst[256] = {0};

    strncpy(src, psrc, sizeof(src) -1);
    strncpy(dst, pdst, sizeof(dst) -1);

    name_size = strlen(src);
    if(name_size > 0 && src[name_size - 1] == '/')
        src[name_size - 1] = 0;

    name_size = strlen(dst);
    if(name_size > 0 && dst[name_size - 1] == '/')
        dst[name_size - 1] = 0;

    if(file_isdir(src))  /*目录*/
    {
        if((dp = opendir(src)) == NULL)
        {
            fprintf(stderr, "opendir '%s' error: %s\n", src, strerror(errno));
            return ;
        }

        strcat(src, "/");
        strcat(dst, "/");
        char *ps = src + strlen(src);
        char *pd = dst + strlen(dst);
        while((dirp = readdir(dp)) != NULL)
        {
            if(strcmp(dirp->d_name, ".") == 0 ||
               strcmp(dirp->d_name, "..") == 0)
                continue;

            strcpy(ps, dirp->d_name);
            strcpy(pd, dirp->d_name);

            if(file_isdir(src))
            {
                if(mkdir(dst, 0) != 0)
                {
                    if(option->f == 0)
                    {
                        printf("do you want overwrite dir '%s' ?", dst);
                        if(!yesno())
                            return;
                    }
                }
                cp_r(src, dst, option, need_del);
            }
            else
            {
                cp_file(src, dst, option, need_del);
            }
        }
    }
    else
    {
        cp_file(src, dst, option, need_del);
    }

    if(need_del){
        struct rm_option rm_op;
        rm_op.f = 1;
        rm_op.r = 1;
        rm_dir(psrc, &rm_op);
    }
}


void
cp( char *psrc,  char *pdst, struct cp2mv_option *option, int needdel)
{
    if(option->r)
        cp_r(psrc, pdst, option, needdel);
    else
        cp_file(psrc, pdst, option, needdel);
}


static void
copy_link(const char *psrc,const char *pdst)
{
    char            dst_name[256] = {0};
    char            result[256] = {0};

    if(readlink(psrc, dst_name, sizeof(dst_name)-1) == -1)
    {
        fprintf(stderr, "readlink file %s error: %s\n", psrc, strerror(errno));
        return ;
    }
    unlink(pdst);
    if(realpath(dst_name, result) != NULL)
    {
        if(symlink(result, pdst) != 0)
            fprintf(stderr, "symlink file '%s' -> '%s'error: %s\n", dst_name, pdst, strerror(errno));
    }
    else
    {
        if(symlink(dst_name, pdst) != 0)
            fprintf(stderr, "symlink file '%s' -> '%s'error: %s\n", dst_name, pdst, strerror(errno));
    }
}


static int
is_link(const char *file)
{
    struct stat         statbuf;

    if(lstat(file, &statbuf) < 0)
        return 0;

    if(S_ISLNK(statbuf.st_mode))
        return 1;

    return 0;
}
/*
    函数功能:
        复制一个文件
    返回值:
        == 0    成功
        != 0    失败(错误码对应errno)
*/
int
copy(const char *src_file,        /* 指向需要复制的源文件                 */
     const char *dst_file,          /* 指向复制文件的目标文件               */
     int  is_force,                 /* 当目标文件存在时，是否是强制覆盖     */
     int  deal_link,                /* 是否要复制链接文件                   */
     int  needdel                   /* 是否需要删除源文件                   */
    )
{
    assert(src_file != NULL && src_file[0] != 0);
    assert(src_file != NULL && src_file[0] != 0);

    int             fdin = -1, fdout = -1;
    void            *psrc = NULL, *pdst = NULL;
    struct stat     statbuf;

    errno = 0;
    if((is_force == 0) && file_isexist(dst_file))  /* 如果目标文件已经存在  */
    {
        printf("do you want to overwrite file '%s' ?(Y/N)\t", dst_file);
        if(yesno() == 0)
        {
            return 0;   /* 不覆盖，返回假成功    */
        }
    }

    if(deal_link && is_link(src_file))
    {
        copy_link(src_file, dst_file);

        return 0;
    }


    if((fdin = open(src_file, O_RDONLY)) < 0)
    {
        fprintf(stderr, "open file '%s' error: %s\n", src_file, strerror(errno));
        return errno;
    }

    if(fstat(fdin, &statbuf) < 0)
    {
        fprintf(stderr, "fstat file '%s' error: %s\n", src_file, strerror(errno));
        close(fdin);
        return errno;
    }

    if((fdout = open(dst_file, O_RDWR | O_CREAT | O_TRUNC)) < 0)
    {
        fprintf(stderr, "can't create file '%s' error: %s\n", dst_file, strerror(errno));
        close(fdin);
        return errno;
    }

    /* 设置输出文件的大小 */
    if(lseek(fdout, statbuf.st_size-1, SEEK_SET) == -1)
    {
        fprintf(stderr, "lseek file '%s' error: %s\n", dst_file, strerror(errno));
        close(fdin);
        close(fdout);
        return errno;
    }
    if(write(fdout, "", 1) != 1)
    {
        fprintf(stderr, "write file '%s' error: %s\n", dst_file, strerror(errno));
        close(fdin);
        close(fdout);
        return errno;
    }

    /* 映射源文件与目标文件  */
    if((psrc = mmap(0, statbuf.st_size, PROT_READ, MAP_PRIVATE, fdin, 0)) == MAP_FAILED)
    {
        fprintf(stderr, "mmap file '%s' error: %s\n", src_file, strerror(errno));
        close(fdin);
        close(fdout);
        return errno;
    }
    if((pdst = mmap(0, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fdout, 0)) == MAP_FAILED)
    {
        fprintf(stderr, "mmap file '%s' error: %s\n", dst_file, strerror(errno));
        close(fdin);
        close(fdout);
        munmap(psrc, statbuf.st_size);
        return errno;
    }

    /* 真正复制文件 */
    memmove(pdst, psrc, statbuf.st_size);

    /* 关闭文件并解除文件映射 */
    close(fdin);
    close(fdout);
    munmap(psrc, statbuf.st_size);
    munmap(pdst, statbuf.st_size);

    if(needdel){
        struct rm_option rm_op;
        rm_op.f = 1;
        rm_op.r = 0;
        return rm_simple(src_file, &rm_op); 
    }
    
    return 0;
}

#endif
