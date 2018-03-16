#ifndef _IS_ROOT_PROCESS_H_
#define _IS_ROOT_PROCESS_H_

#include <unistd.h>

#if 0
/*��������жϲ�����su��ȥ��root�û�*/
int
is_root_process()
{
    char        *ptr = NULL;

    if((ptr = getenv("LOGNAME")) == NULL)
        return 0;
    if(strcmp(ptr, "root") == 0)
        return 1;

    return 0;
}
#endif

int
is_root_process()
{
    if(geteuid() != 0)
        return 0;
    
    return 1;
}
#endif

