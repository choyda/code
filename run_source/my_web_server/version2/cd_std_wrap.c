#include <stdio.h>
#include <errno.h>

/*���ù��ܺ�����װ*/

void perr_exit(const char *s)
{
    perror(s);
    exit(-1);
}