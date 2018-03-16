/*
    ������ģ��cat����(ֻģ���ӡ�������������ַ�)

    ��ʷ��¼:
    ����            ����            ����            ��ϵ��ʽ
    20140417        ¬����          �״δ���        1164830775@qq.com

    ����:
    ����ֻ�ǽ����ݴ�ӡ����׼��������������е������ַ����д���
    ���û�����κβ���������������ļ���Ϊ-,��ӱ�׼���������
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

static char             *program_name = NULL;   /* ������ָ��               */
static char             *infile = NULL;         /* ָ�������ļ�             */
static int              input_desc = -1;        /* �����ļ�������           */
static int              exit_status = EXIT_FAILURE; /* �����˳�״̬             */

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

    if(file[0] == '-' && file[1] == 0)/*�ӱ�׼���������*/
        _cat(STDIN_FILENO, 1);   /*�ӱ�׼��������ݲ���ʾ���ݣ�Ȼ���˳�*/

    /*�����Ǵ�file�ļ������������*/
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
