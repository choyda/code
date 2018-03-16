#ifndef _SAFE_READ_H_
#define _SAFE_READ_H_


#include <sys/types.h>
#include <errno.h>
#include <unistd.h>

static size_t 
safe_read(int desc, void *ptr, size_t len)
{
    size_t          n_chars;

    if(len <= 0)
        return len;
#ifdef EINTR
    do{
        n_chars = read(desc, ptr, len);
    }while(n_chars < 0 && errno == EINTR);
#else
    n_chars = read(desc, ptr, len);
#endif

    return n_chars;
}


#endif

