/*
    ���ļ��ӱ�׼��������Ƿ���yes��no,ֱ������yes����no���߳����˳��ŷ���
*/
#ifndef _COMMAND_YES_NO_H_
#define _COMMAND_YES_NO_H_

#include <stdio.h>
#include <ctype.h>

/*
    ����ֵ:
        ==  1 �ӱ�׼�������yes
        ==  0 �ӱ�׼���������no
*/
int yesno()
{
    char buf[128];
    int len = 0;
    int c;


    buf[0] = 'n';
    while((c = getchar()) != EOF && c != '\n')
        if((len > 0 && len < 127) || (len == 0 && !isspace (c)))    /* ���������ǰ��Ŀո� */
            buf[len++] = c;

    buf[len] = 0;
    if(buf[0] == 'y' || buf[0] == 'Y')
        return 1;

    return 0;
}

#endif
