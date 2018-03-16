/*
    本程序模拟cat命令(只模拟打印，不处理特殊字符)

    历史记录:
    日期            作者            描述            联系方式
    20140417        卢富波          首次创建        1164830775@qq.com

    需求:
    仅仅只是将数据打印到标准输出，不对数据中的特殊字符进行处理
    如果没输入任何参数，或者输入的文件名为-,则从标准输入读数据
*/
#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#include <getopt.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define     EXIT_FAILURE            1
#define     EXIT_SUCESS             0
#define     MAX_BUF_SIZE            1024

static char             *program_name = NULL;   /* 程序名指针               */
static char             *infile = NULL;         /* 指向输入文件             */
static int              input_desc = -1;        /* 输入文件描述符           */
static int              exit_status = EXIT_FAILURE; /* 命令退出状态             */

static void
usage(int status)
{
    if(status != 0){
        fprintf(stderr, "Try '%s --help' for more infomation\n",
            program_name);
    }else{
        printf("Usage: %s  [FILE] ...\n", program_name);
        printf("with no FILE or when FILE is -, %s will read from stdin\n", program_name);
    }

    exit(status);
}

static void
cat_exit()
{
    exit(exit_status);
}

static void
_cat(int fileno, int needexit)
{
    char                buf[MAX_BUF_SIZE];
    int                 n;
    
    while((n = read(fileno, buf, MAX_BUF_SIZE)) > 0){
        if(write(STDOUT_FILENO, buf, n) != n){
            perror("in write");
            exit(exit_status);
        }
    }

    if(n < 0){
        perror("in read");
        if(!needexit)
            exit(exit_status);
    }

    if(needexit){
        exit_status = EXIT_SUCESS;
        exit(exit_status);
    }
}

static void
simple_cat(char *file)
{
    struct stat         statbuf;
    
    if(file == NULL || file[0] == 0){
        //sprintf(stderr, "some asset error happend\n");
        exit(exit_status);
    }

    if(file[0] == '-' && file[1] == 0)/*从标准输入读数据*/
        _cat(STDIN_FILENO, 1);   /*从标准输入读数据并显示数据，然后退出*/

    /*下面是从file文件里面读数据了*/
    input_desc = open(file, O_RDONLY);
    if(input_desc < 0){
        fprintf(stderr, "open file '%s' error: %s\n", file, strerror(errno));
        exit(exit_status);
    }

    if(fstat(input_desc, &statbuf) < 0){
        fprintf(stderr, "fstat file '%s' error: %s\n", file, strerror(errno));
        close(input_desc);
        exit(exit_status);
    }

    if(S_ISREG(statbuf.st_mode) == 0){
        fprintf(stderr, "file '%s' is not a genel file\n", file);
        close(input_desc);
        exit(exit_status);
    }

    _cat(input_desc, 0);
    close(input_desc);
}

int
main(int argc, char *argv[])
{
    struct stat         stat_buf;
    int                 c;
    static struct option const long_options[] =
    {
        {"help", 0, NULL, 'h'},
            {NULL, 0, NULL, 0}
    };

    program_name = argv[0];
    atexit(cat_exit);

    while((c = getopt_long(argc, argv,
        "h", long_options, NULL)) != -1){
        switch(c)
        {
            case 'h':
                usage(EXIT_SUCESS);
                break;
            default:
                usage(EXIT_FAILURE);
                break;
        }
    }
    
    if(fstat(STDOUT_FILENO, &stat_buf) < 0){
        perror("fstat STDOUT_FILENO");
        exit(exit_status);
    }
    
    if(optind < argc){
        while(optind < argc){
            infile = argv[optind++];
            simple_cat(infile);
        }
    }else{
        infile = "-";
        simple_cat(infile);
    }

    exit_status = EXIT_SUCESS;

    exit(exit_status);
}
