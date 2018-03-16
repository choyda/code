#ifndef _IS_DIGIT_H_
#define _IS_DIGIT_H_



/*
    判断是否为满足要求
    返回值
    ==  0   不满足要求
    ==  1   满足要求
*/
int
is_in(char *ptr,                /*  待检测字符串                   */
      size_t size,             /*  字符串长度，不包含结尾符   */
      int min,                /*  最小值(包含)                */
      int max                 /*最大值(包含)                  */
     )
{
    int                 i;

    if(ptr == NULL || size == 0 || min > max)
        return 0;

    for(i = 0; i < size; ++i)
    {
        if( ptr[i] > max ||
            ptr[i] < min)
            return 0;
    }

    return 1;
}

/*
    判断是否为数据字符串
    返回值
    ==  0   不全为数字
    ==  1   全为数字
*/
int
is_digit(char *ptr,     /*  待检测字符串             */
         size_t size    /*  字符串长度，不包含结尾符 */
        )
{
    return is_in(ptr, size, '0', '9');
}

#endif
