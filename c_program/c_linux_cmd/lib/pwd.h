#ifndef _PWD_H_
#define _PWD_H_

#include <unistd.h>
#include <errno.h>
#include <stdlib.h>


/*
    得到当前目录

    返回值:
        ==  NULL  失败
        !=  NULL  成功

    FIXME
        返回的内存是堆内存，需要调用者释放
*/
char *
get_pwd()
{
    size_t          path_size = 128;
    char            *ret;
    
    while(1){
        if((ret = (char *)malloc(path_size)) == NULL)
            return NULL;
        
        if(getcwd(ret, path_size) != NULL)
            return ret;

        if(errno != ERANGE){
            free(ret);

            return NULL;
        }else{
            path_size = path_size * 2;
            free(ret);
        }

        if(path_size == 0)
            return NULL;
    }
}

#endif
