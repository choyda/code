/*
    ��maskֵ�Ļ�����������
*/
#ifndef _UMASK_H_
#define _UMASK_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "./is_digit.h"

/*
    �õ���ǰ��mode_tֵ
*/
mode_t
get_mode()
{
    mode_t          ret;

    ret = umask(0);
    umask(ret);         /*  ��ԭumaskֵ */

    return ret;
}

/*
    ����mode_tֵ
*/
void
set_mode(mode_t newmod)
{
    umask(newmod);
}

/*
    ��mode_tת��Ϊ�ַ�����ʽ
        ���������10λ������õ�9λ������������λ(ճ��λ)
    ����ֵ:
    ==0     �ɹ�
    !=0     ʧ��
*/
int
mode_to_str(mode_t mode,    /*  ��ת����mode_t����  */
char *ret,                      /*  ת������ַ���ָ��  */
size_t ret_size                 /*  ָ����ָ����        */
)
{
    int                 i;
    const int           min_size = 5;   /*�������С����*/

    if(ret == NULL || ret_size < min_size)
        return -1;

    /*����ǰ22λ�Ƿ���1*/
    for(i = 10; i < 32; ++i)
    {
        if((1 << i) & mode)
        {
            fprintf(stderr, "mode outof range\n");
            return -2;
        }
    }

    snprintf(ret, 2, "%d", mode>>9); /*ȡ��10λ*/
#if 0
    snprintf(ret+1, 1, "%d", 7&(tmp>>6));
    snprintf(ret+2, 1, "%d", 7&(tmp>>3));
    snprintf(ret+3, 1, "%d", 7&(tmp>>0));
#endif
    /*����ȡ��λ*/
    for(i = 1; i <= 3; ++i)
        snprintf(ret+i, 2, "%d", 7&(mode>>(9-i*3)));

    ret[i] = 0;

    return 0;
}


/*
    ���ַ�����ʽת��Ϊmode_t��ʽ
        ���������9λ������õ�9λ������������λ(ճ��λ)
    ����ֵ:
    ==0     �ɹ�
    !=0     ʧ��
*/
int
str_to_mode(mode_t *mode,               /*  ���ص�mode_t����        */
            char *mode_str,                 /*  �ַ�����ʽ��mode_t����  */
            size_t str_size                 /*  �ַ������ȣ���������β��*/
           )
{
    int                 i, tmp, tmp3bit;

    assert(mode != NULL);
    if(is_in(mode_str, str_size, '0', '7') == 0 ||
       str_size > 4)
    {
        fprintf(stderr, "error mode str [%s]\n", mode_str);
        return -1;
    }

    tmp = atoi(mode_str);
    if(tmp > 1777)
    {
        fprintf(stderr, "error mode str [%s]\n", mode_str);
        return -2;
    }

    memset(mode, 0, sizeof(mode_t));
    for(i = 0; i < 4; ++i){
        tmp3bit = tmp%10;
        tmp = tmp/10;
        *mode = (*mode)|(tmp3bit<<(i*3));
    }

    return 0;
}




#endif
