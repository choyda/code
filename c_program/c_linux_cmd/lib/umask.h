/*
    对mask值的基本操作集合
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
    得到当前的mode_t值
*/
mode_t
get_mode()
{
    mode_t          ret;

    ret = umask(0);
    umask(ret);         /*  还原umask值 */

    return ret;
}

/*
    设置mode_t值
*/
void
set_mode(mode_t newmod)
{
    umask(newmod);
}

/*
    将mode_t转换为字符串形式
        仅处理最低10位，即最常用的9位，不包括特殊位(粘贴位)
    返回值:
    ==0     成功
    !=0     失败
*/
int
mode_to_str(mode_t mode,    /*  待转换的mode_t类型  */
char *ret,                      /*  转换后的字符串指针  */
size_t ret_size                 /*  指针所指长度        */
)
{
    int                 i;
    const int           min_size = 5;   /*传入的最小长度*/

    if(ret == NULL || ret_size < min_size)
        return -1;

    /*检验前22位是否含有1*/
    for(i = 10; i < 32; ++i)
    {
        if((1 << i) & mode)
        {
            fprintf(stderr, "mode outof range\n");
            return -2;
        }
    }

    snprintf(ret, 2, "%d", mode>>9); /*取第10位*/
#if 0
    snprintf(ret+1, 1, "%d", 7&(tmp>>6));
    snprintf(ret+2, 1, "%d", 7&(tmp>>3));
    snprintf(ret+3, 1, "%d", 7&(tmp>>0));
#endif
    /*依次取三位*/
    for(i = 1; i <= 3; ++i)
        snprintf(ret+i, 2, "%d", 7&(mode>>(9-i*3)));

    ret[i] = 0;

    return 0;
}


/*
    将字符串形式转换为mode_t形式
        仅处理最低9位，即最常用的9位，不包括特殊位(粘贴位)
    返回值:
    ==0     成功
    !=0     失败
*/
int
str_to_mode(mode_t *mode,               /*  返回的mode_t类型        */
            char *mode_str,                 /*  字符串形式的mode_t类型  */
            size_t str_size                 /*  字符串长度，不包括结尾符*/
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
