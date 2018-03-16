/*
    在写linux下命令时，大部份程序会用到的一些参数
*/
#ifndef _COMMAND_COMM_H_
#define _COMMAND_COMM_H_
#include <stdio.h>



#define EXIT_SUCESS             0           /* 正常退出状态         */
#define EXIT_FAILURE            1           /* 非正常退出状态       */
#define MAX_LINE_BUF_SIZE       1024        /* 默认的行缓冲大小     */

static char    *program_name = NULL;        /* 程序名               */
static int     exit_status = EXIT_FAILURE;  /* 程序退出状态         */



#endif
