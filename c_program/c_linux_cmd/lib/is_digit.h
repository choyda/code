#ifndef _IS_DIGIT_H_
#define _IS_DIGIT_H_



/*
    �ж��Ƿ�Ϊ����Ҫ��
    ����ֵ
    ==  0   ������Ҫ��
    ==  1   ����Ҫ��
*/
int
is_in(char *ptr,                /*  ������ַ���                   */
      size_t size,             /*  �ַ������ȣ���������β��   */
      int min,                /*  ��Сֵ(����)                */
      int max                 /*���ֵ(����)                  */
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
    �ж��Ƿ�Ϊ�����ַ���
    ����ֵ
    ==  0   ��ȫΪ����
    ==  1   ȫΪ����
*/
int
is_digit(char *ptr,     /*  ������ַ���             */
         size_t size    /*  �ַ������ȣ���������β�� */
        )
{
    return is_in(ptr, size, '0', '9');
}

#endif
