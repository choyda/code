#include <stdio.h>

int main(int argc, char *argv[])
{   
    //1. 指定创建子进程的个数

    //2. 打开源文件
    
    //3. 打开目的文件, 不存在则创建
    
    //4. 获取文件大小
    
    //5. 根据文件大小拓展目标文件
    
    //6. 为源文件创建映射
    
    //7. 为目标文件创建映射
    
    //8. 求出每个子进程该拷贝的字节数

    //9. 创建N个子进程

    //10. 子进程完成分块拷贝(注意最后一个子进程拷贝起始位置)

    //11. 释放映射区

    return 0;


    在计算机科学中,信号(英语:Signals)是 Unix、类 Unix 以及其他 POSIX 兼容的操作系统中进程间通讯的一种有限制的方式。它是一种异步的通知机制,用来提醒进程一个事件已经发生。当一个信号发送给一个进程,操作系统中断了进程正常的控制流程,此时,任何非原子操作都将被中断。如果进程定义了信号的处理函数,那么它将被执行,否则就执行默认的处理函数。
}




1. 信号是什么:UNIX 进程间的一种异步通讯机制。
2. 信号的作用:提醒目标进程某事件的发生,并中断了目标进程正常的控制流程。
3. 进程对信号的处理:目标进程可以执行信号的默认处理函数,或者执行进程自定义的信号处理程序。

每个信号都有一个名字, 这些名字都以三个字符 SIG 开头。 例如,SIGABRT是夭折信号, 当进程调用 abort 函数时产生这种信号。SIGALRM 是闹钟信号,
当由 alarm 函数设置的计时器超时后产生此信号。在 UNIX 系统中,这些信号都定义在头文件 <signal.h> 中,并且都是以一个正整数来表示(信号编号)。
通过在 shell 中运行命令 kill -l 可以查看当前系统所执行的所有信号

2018-01-01,2018-09-07,1|2018-01-01,2018-09-03,2|2018-01-01,2018-09-03,2

ALTER TABLE `hotel_allota_order` ADD INDEX (`openudid`);

 $oApiCache = CMemcachedManager::getInstance();error: 201712/hotel/caida/touge_discoun_update1.php: patch does not apply


   //hahah
 19 # ---
 20 # To remove '-' lines, make them ' ' lines (context).
 21 # To remove '+' lines, delete them.
 22 # Lines starting with # will be removed.
 23 #
 24 # If the patch applies cleanly, the edited hunk will immediately be
 25 # marked for staging. If it does not apply cleanly, you will be given
 26 # an opportunity to edit again. If all lines of the hunk are removed,
 27 # then the edit is aborted and the hunk is left unchanged.

        $cMemQueryInfo = $oApiCache->get($this->memKeyQuery());

         $oApiCache->set($this->memKeyQuery(), $menRes, 43200);}


产生信号:

1. 按键产生，如：Ctrl+c、Ctrl+z、Ctrl+\
2. 系统调用产生，如：kill、raise、abort 函数
3. 软件条件产生，如：定时器alarm函数
4. 硬件异常产生，如：非法访问内存(段错误)、除0(浮点数例外)、内存对齐出错(总线错误)
5. 命令产生，如：kill命令

终端按键产生信号undefined
Ctrl + c  → 2) SIGINT（终止/中断）       
Ctrl + \  → 3) SIGQUIT（退出）  
Ctrl + z  → 20) SIGTSTP（暂停/停止） 
   
硬件异常产生信号undefined
除0操作   → 8) SIGFPE (浮点数例外)  
非法访问内存  → 11) SIGSEGV (段错误)
总线错误  → 7) SIGBUS

"F" -----float 浮点数。
"T" ----Terminal 终端。
"INT" ----Interrupt
