/*
    echo.c
    ����:
        ģ��echo����
    ��ʷ��¼:
        ����        ʱ��        ����        ��ϵ��ʽ
        ¬����      2014-05-18  ����        1164830775@qq.com

    FIXME
        ���������ӡ�ַ�������
        �о�����echo $HOME��ӡ�����$HOME,SHELL����Ὣ�䷭�����ٴ����
*/

#include <stdio.h>
#include <stdlib.h>


int
main(int argc, char *argv[])
{
    int         beg = 1;
    
    while(beg < argc){
        fprintf(stdout, argv[beg++]);
        if(argc != beg)     /*  �����м�Ŵ�ӡ�ո�  */
            fprintf(stdout, " ");
        }

    fprintf(stdout, "\n");

    exit(0);
}

