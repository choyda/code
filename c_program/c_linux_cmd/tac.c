/*
    功能:
        模拟tac命令
    需求:
        仅仅将文件倒顺打印到标准输出，如果有换行符，则按换行符倒序，如果是类似二进制的文件，则按缓冲区大小来倒序打印
    历史记录:
        日期        作者        操作            联系方式
        20140417    卢富波      首次创建        1164830775@qq.com

    FIXME:
        1   此程序用了mmap，这样方便处理。
        2   在考虑倒序输出时，本程序认为较好的方式是从文件最后开始遍历，而不是从开始处遍历
            从后面开始遍历的好处是，边界条件要少考虑很多，不容易出错
*/

#include "lib/command_comm.h"
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <assert.h>
#include <unistd.h>


#define     SEP_CHAR        '\n'    /*默认的分隔符      */


static void
tac_exit(void)
{

}

static void
usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s  FILE ...\n", program_name);
        printf("Input at least one file.And the file must be normal file\n");
    }

    exit(status);
}

static void
write_size(void *buf, size_t size, int fileno)
{
    if(size == 0)
        return;

    if(write(fileno, buf, size) != size)
    {
        perror("write");
        close(fileno);
        exit(EXIT_FAILURE);
    }
}


static void
deal_mem(char *pdata, size_t size)
{
    assert(pdata != NULL);
    int                      i, line_size = 0;

    for(i = size - 1; i >= 0; --i)
    {
        ++line_size;
        if(pdata[i] == SEP_CHAR)        /*找到一个分隔符*/
        {
            write_size((void *)(pdata+i), line_size, STDOUT_FILENO);   /*写这一行数据*/
            line_size = 0;
        }
    }

    if(line_size != 0)          /*开始处(除开第一个字节)的数据*/
    {
        write_size("\n", 1, STDOUT_FILENO);/*先写个换行符*/
        write_size((void *)pdata, line_size, STDOUT_FILENO);
    }
    write_size("\n", 1, STDOUT_FILENO);
}

static void
_tac(int tac_fileno, size_t file_size)
{
    void                *p_mmap;

    if((p_mmap = mmap(0, file_size,
                      PROT_READ, MAP_PRIVATE,
                      tac_fileno, 0)) == MAP_FAILED)
    {
        perror("mmap");
        close(tac_fileno);
        exit(EXIT_FAILURE);
    }

    deal_mem((char *)p_mmap, file_size);

    if(munmap(p_mmap, file_size) < 0)
    {
        perror("munmap");
        close(tac_fileno);
        exit(EXIT_FAILURE);
    }

    close(tac_fileno);
}


static void
tac(char *file)
{
    int                 f_desc = -1;
    struct stat         statbuf;

    if(file == NULL || file[0] == 0)
    {
        fprintf(stderr, "some assert head happend\n");
        exit(EXIT_FAILURE);
    }

    if((f_desc = open(file, O_RDONLY)) < 0)
    {
        fprintf(stderr, "open file '%s' error: %s\n", file, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if(fstat(f_desc, &statbuf) < 0)
    {
        fprintf(stderr, "fstat file '%s' error: %s\n", file, strerror(errno));
        close(f_desc);
        exit(EXIT_FAILURE);
    }

    if(S_ISREG(statbuf.st_mode) == 0)
    {
        fprintf(stderr, "file '%s' is not a normal file\n", file);
        close(f_desc);
        exit(EXIT_FAILURE);
    }

    if(statbuf.st_size == 0)
        return;

    _tac(f_desc, statbuf.st_size);
}

int
main(int argc, char *argv[])
{
    int                     c;
    static struct option const long_options[] =
    {
        {"help", 0, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };

    program_name = argv[0];
    atexit(tac_exit);

    while((c = getopt_long(argc, argv,
                           "h", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
                usage_exit(EXIT_SUCESS);
                break;
            default:
                usage_exit(EXIT_FAILURE);
                break;
        }
    }

    if(optind < argc) /*这些文件是要tac的文件列表*/
    {
        while(optind < argc)
            tac(argv[optind++]);
    }
    else
    {
        usage_exit(EXIT_FAILURE);
    }

    exit_status = EXIT_SUCESS;
    exit(exit_status);
}
