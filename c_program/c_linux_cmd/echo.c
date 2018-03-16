/*
    echo.c
    功能:
        模拟echo命令
    历史记录:
        作者        时间        操作        联系方式
        卢富波      2014-05-18  创建        1164830775@qq.com

    FIXME
        这里仅仅打印字符串东西
        感觉类似echo $HOME打印，这个$HOME,SHELL命令会将其翻译了再传入的
*/

#include <stdio.h>
#include <stdlib.h>


int
main(int argc, char *argv[])
{
    int         beg = 1;
    
    while(beg < argc){
        fprintf(stdout, argv[beg++]);
        if(argc != beg)     /*  仅在中间才打印空格  */
            fprintf(stdout, " ");
        }

    fprintf(stdout, "\n");

    exit(0);
}

