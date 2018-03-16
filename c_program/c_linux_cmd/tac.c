/*
    ����:
        ģ��tac����
    ����:
        �������ļ���˳��ӡ����׼���������л��з����򰴻��з�������������ƶ����Ƶ��ļ����򰴻�������С�������ӡ
    ��ʷ��¼:
        ����        ����        ����            ��ϵ��ʽ
        20140417    ¬����      �״δ���        1164830775@qq.com

    FIXME:
        1   �˳�������mmap���������㴦��
        2   �ڿ��ǵ������ʱ����������Ϊ�Ϻõķ�ʽ�Ǵ��ļ����ʼ�����������Ǵӿ�ʼ������
            �Ӻ��濪ʼ�����ĺô��ǣ��߽�����Ҫ�ٿ��Ǻܶ࣬�����׳���
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


#define     SEP_CHAR        '\n'    /*Ĭ�ϵķָ���      */


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
        if(pdata[i] == SEP_CHAR)        /*�ҵ�һ���ָ���*/
        {
            write_size((void *)(pdata+i), line_size, STDOUT_FILENO);   /*д��һ������*/
            line_size = 0;
        }
    }

    if(line_size != 0)          /*��ʼ��(������һ���ֽ�)������*/
    {
        write_size("\n", 1, STDOUT_FILENO);/*��д�����з�*/
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

    if(optind < argc) /*��Щ�ļ���Ҫtac���ļ��б�*/
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
