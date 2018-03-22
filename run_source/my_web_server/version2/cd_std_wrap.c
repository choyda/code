#include <stdio.h>
#include <errno.h>

/*常用功能函数封装*/

void perr_exit(const char *s)
{
    perror(s);
    exit(-1);
}