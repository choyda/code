#ifndef _LINK_H_
#define _LINK_H_

#include <unistd.h>
#include <errno.h>
#include <string.h>

int
create_hardlink(const char *existingpath, const char *newpath)
{
    if(link(existingpath, newpath) != 0)
    {
        fprintf(stderr,
                "create head link [%s] [%s] error[%s]\n",
                existingpath, newpath, strerror(errno));
        return errno;
    }

    return 0;
}

int
create_softlink(const char *existingpath, const char *newpath)
{
    if(symlink(existingpath, newpath) != 0)
    {
        fprintf(stderr,
                "create soft link [%s] [%s] error[%s]\n",
                existingpath, newpath, strerror(errno));
        return errno;
    }

    return 0;
}

#endif

