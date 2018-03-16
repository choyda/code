#include <sys/ioctl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#define _IOC_WRITE  1U
#define _IOC_READ   2U

#define _IOC_NRBITS	8
#define _IOC_TYPEBITS	8
#define _IOC_SIZEBITS	14
#define _IOC_DIRBITS	2

#define _IOC_NRSHIFT	0
#define _IOC_TYPESHIFT	(_IOC_NRSHIFT+_IOC_NRBITS)
#define _IOC_SIZESHIFT	(_IOC_TYPESHIFT+_IOC_TYPEBITS)
#define _IOC_DIRSHIFT	(_IOC_SIZESHIFT+_IOC_SIZEBITS)

#define _IOC(dir,type,nr,size) \
	(((dir)  << _IOC_DIRSHIFT) | \
	 ((type) << _IOC_TYPESHIFT) | \
	 ((nr)   << _IOC_NRSHIFT) | \
	 ((size) << _IOC_SIZESHIFT))

#define _MY_IOR(type,nr,size)      _IOC(_IOC_READ,(type),(nr),sizeof(size))
#define _MY_IOW(type,nr,size)      _IOC(_IOC_WRITE,(type),(nr),sizeof(size))

#define EXT2_IOC_GETFLAGS       _MY_IOR('f', 1, long)
#define EXT2_IOC_SETFLAGS       _MY_IOW('f', 2, long)

int _fgetflags (const char * name, unsigned int *flags)
{
    struct stat buf;
    int fd, r, f, save_errno = 0;

    if (!lstat(name, &buf) &&
        !S_ISREG(buf.st_mode) && !S_ISDIR(buf.st_mode))
    {
        return 1;
    }

    fd = open (name, O_RDONLY|O_NONBLOCK);
    if (fd == -1)
        return 2;
    r = ioctl (fd, EXT2_IOC_GETFLAGS, &f);
    if (r == -1)
        save_errno = errno;
    *flags = f;
    close (fd);
    if (save_errno)
        errno = save_errno;
    return r;
}

int _fsetflags (const char * name, unsigned int flags)
{

    int fd, r, f, save_errno = 0;
    struct stat buf;

    if (!lstat(name, &buf) &&
        !S_ISREG(buf.st_mode) && !S_ISDIR(buf.st_mode))
    {
        return 1;
    }
    fd = open (name, O_RDONLY|O_NONBLOCK);
    if (fd == -1)
        return 2;
    f = (int) flags;
    r = ioctl (fd, EXT2_IOC_SETFLAGS, &f);
    if (r == -1)
        save_errno = errno;
    close (fd);
    if (save_errno)
        errno = save_errno;

    return r;
}

