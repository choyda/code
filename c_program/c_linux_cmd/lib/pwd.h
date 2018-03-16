#ifndef _PWD_H_
#define _PWD_H_

#include <unistd.h>
#include <errno.h>
#include <stdlib.h>


/*
    �õ���ǰĿ¼

    ����ֵ:
        ==  NULL  ʧ��
        !=  NULL  �ɹ�

    FIXME
        ���ص��ڴ��Ƕ��ڴ棬��Ҫ�������ͷ�
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
