/*
    该文件从标准输入读入是否是yes与no,直到读到yes或者no或者程序退出才返回
*/
#ifndef _COMMAND_YES_NO_H_
#define _COMMAND_YES_NO_H_

#include <stdio.h>
#include <ctype.h>

/*
    返回值:
        ==  1 从标准输入读了yes
        ==  0 从标准输入读入了no
*/
int yesno()
{
    char buf[128];
    int len = 0;
    int c;


    buf[0] = 'n';
    while((c = getchar()) != EOF && c != '\n')
        if((len > 0 && len < 127) || (len == 0 && !isspace (c)))    /* 这里除掉了前面的空格 */
            buf[len++] = c;

    buf[len] = 0;
    if(buf[0] == 'y' || buf[0] == 'Y')
        return 1;

    return 0;
}

#endif
