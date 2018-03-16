/*
    chattr.c
    功能:
        模拟chattr命令:设置文件隐藏属性(暂时只考虑a i属性)
    历史记录:
        作者        时间        操作        联系方式
        卢富波      2014-05-16  创建        1164830775@qq.com
*/

#include "lib/command_comm.h"
#include "lib/fileflag.h"
#include "lib/file_isexist.h"
#include "lib/is_root_process.h"
#include "lib/bit_opr.h"
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>

enum user_op
{
    NO_OPTION = 0,
    ADD_OPTION = 1,
    DEL_OPTION = -1
};

struct chattr_opt
{
    enum user_op        a;
    enum user_op        i;
};

static void
init_opt(struct chattr_opt *op)
{
    op->a = NO_OPTION;
    op->i = NO_OPTION;
}

static void
chattr_usage_exit(int status)
{
    if(status != 0)
    {
        fprintf(stderr, "Try '%s --help' for more infomation\n",
                program_name);
    }
    else
    {
        printf("Usage: %s  FILE\n", program_name);
        printf("+a              add append attribution\n");
        printf("+i              add immutable attribution\n");
        printf("-a              del append attribution\n");
        printf("-i              del immutable attribution\n");
        printf("-h(-H)  --help  show help information\n");
        printf("\nNOTE          you must run as root\n");
    }

    exit(status);
}

static void
judge_ttr(struct chattr_opt *op, unsigned int *attr)
{
#define I_OFFSET   4
#define A_OFFSET   5
    if(op->a == ADD_OPTION)
        BIT_SET(*attr, A_OFFSET);
    if(op->i == ADD_OPTION)
        BIT_SET(*attr, I_OFFSET);

    if(op->a == DEL_OPTION)
        BIT_CLR(*attr, A_OFFSET);
    if(op->i == DEL_OPTION)
        BIT_CLR(*attr, I_OFFSET);
#undef A_OFFSET
#undef I_OFFSET
}

static int
ch_file_attr(const char *file, struct chattr_opt *op)
{
    unsigned int        attr;
    int                 err;

    if(file_isexist(file) == 0)
    {
        fprintf(stderr,
                "file [%s] does not existed\n",
                file);
        return 1;
    }

    if((err = _fgetflags(file, &attr)) != 0)
        return err;

    judge_ttr(op, &attr);

    if((err = _fsetflags(file, attr)) != 0)
        return err;

    return 0;
}

static int
get_usr_op(char *op, size_t size, enum user_op e_op, struct chattr_opt *option)
{
    size_t          i;

    for(i = 0; i < size; ++i)
    {
        if(op[i] != 'a' && op[i] != 'i')
        {
            fprintf(stderr,
                    "can't analysis option[%c]\n",
                    op[i]);
            return -1;
        }

        if(op[i] == 'a')
            option->a = e_op;
        if(op[i] == 'i')
            option->i = e_op;
    }

    return 0;
}

int
main(int argc, char *argv[])
{
    int                 c;
    struct chattr_opt   op;

    static struct option const long_options[] =
    {
        {"help", 0, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };
    exit_status = 0;
    program_name = argv[0];
    init_opt(&op);
    
    while((c = getopt_long(argc, argv,
                           "hHai", long_options, NULL)) != -1)
    {
        switch(c)
        {
            case 'h':
            case 'H':
                chattr_usage_exit(EXIT_SUCESS);
                break;
            case 'a':
                op.a = DEL_OPTION;
                break;
            case 'i':
                op.i = DEL_OPTION;
                break;
            default:
                chattr_usage_exit(EXIT_FAILURE);
                break;
        }
    }

    if(argv[optind] != NULL && argv[optind][0] == '+')
    {
        if(get_usr_op(argv[optind]+1, strlen(argv[optind])-1, ADD_OPTION, &op) != 0)
            chattr_usage_exit(EXIT_FAILURE);
        optind++;
    }
    
    if(!is_root_process()){
        fprintf(stderr, 
            "must run as root\n");
        chattr_usage_exit(EXIT_FAILURE);
    }
    
    if(argc == optind )
        chattr_usage_exit(EXIT_FAILURE);

    while(optind < argc)
        exit_status |= ch_file_attr(argv[optind++], &op);

    if(exit_status)
    {
        if(exit_status > 0)
            fprintf(stderr,
                    "[%s] error with errno msg[%s]\n",
                    program_name, strerror(exit_status));
        else
            fprintf(stderr,
                    "[%s] error with errno no[%d]\n",
                    program_name, exit_status);
    }

    exit(exit_status);
}

