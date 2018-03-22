#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
/*
 * DESCRIPTION:
 * �����̵Ŀͻ��˽��������ܿ�������ͻ��ˡ��¸��汾����Ӷ����
 */

#define SERV_PORT 9999 //����������˿�
#define SIZEBUF 1024 //buf��С ���1500

void act_sin_fun(int sin_num)
{
    perror("child wait");
    while (waitpid(0, NULL, WNOHANG) > 0)
        ;
}

int main(int argc, char *argv[]){

    /*����ʹ���еı���*/
    int sfd, cfd;                               //�������˺Ϳͻ��˵�socket������
    char buf[SIZEBUF];                          //��ȡд��buf�Ĵ�С
    int len, i;                                 //��ȡ���Ⱥ�ѭ������
    struct sockaddr_in serv_addr, cli_addr;     //�ͻ��˺ͷ�������bind�ṹ��
    char clie_ip[SIZEBUF], serv_ip[SIZEBUF];    //�����ӡ��Ϣip���ַ�����
    struct sigaction signal_act;                //�����źŵĽṹ��
    int pid;                                    //����id

    /*����socket������*/
    sfd = socket(AF_INET, SOCK_STREAM, 0);      //���һ������0���ں˻��Զ����ݳ�ʹ�õ�Э��
    if(sfd == -1){
        perror("fail to create socket");
        exit(1);
    }

    /*�˿ڸ��ã�TIME_WAIT�ȴ�����*/
    int opt = 1;
    if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt)) == -1){
        perror("fail to setsockopt socket");
        exit(1);
    }

    /*��ʼ���Žṹ�壬�����ӽ��̻��շ����źţ�Ҳ������signal�������棬���ǲ����顣*/
    signal_act.sa_handler = act_sin_fun;        //ע���źŴ�����
    sigemptyset(&signal_act.sa_mask);           //����źŵ�mask��
    signal_act.sa_flags = 0;                    //ͨ������Ϊ0����ʹ��Ĭ�����ԡ�
    sigaction(SIGCHLD, &signal_act, NULL);      //Ҫ������ź�,SIGCHLD �����ӽ����˳���


    /*�󶨷�������ַ�ṹ*/
    socklen_t serv_len, cli_len;                //�ṹ�峤��
    serv_len = sizeof(serv_addr);               //��ȡ�ṹ�峤��
    memset(&serv_addr, 0, serv_len);            //��սṹ��
    serv_addr.sin_family = AF_INET;             //ʹ�õ�Э����
    serv_addr.sin_addr.s_addr = INADDR_ANY;     //�������κ�����
    serv_addr.sin_port = htons(SERV_PORT);      //����˿ںţ�����ת�����ֽ���Ϊ0����ϵͳ�Զ����䣬ʹ��getsockname������ϡ�

    /*��ʼ��һ����ַ�ṹ man 7 ip �鿴��Ӧ��Ϣ*/
    if(bind(sfd, (struct sockaddr *)&serv_addr, serv_len) == -1){
        perror("fail to bind socket");
        exit(1);
    }

    /*�趨��������,ע��˴�������*/
    if(listen(sfd, 10) == -1){                   //ͬһʱ������������������������������
        perror("fail to listen socket");
        exit(1);
    }


    /*��ӡһЩ������Ϣ*/
    printf("client IP:%s\tport:%d\t%d\n",
           inet_ntop(AF_INET, &cli_addr.sin_addr.s_addr, clie_ip, sizeof(clie_ip)),
           ntohs(cli_addr.sin_port), cfd);
    printf("server IP:%s\tport:%d\t%d\n",
           inet_ntop(AF_INET, &serv_addr.sin_addr.s_addr, serv_ip, sizeof(serv_ip)),
           ntohs(serv_addr.sin_port), sfd);

    while (1) {
        again:
        if ((cfd = accept(sfd, (struct sockaddr *)&cli_addr, &cli_len)) < 0) {
            if ((errno == ECONNABORTED) || (errno == EINTR))
                goto again;
            else
                perror("adfafas");
                exit(1);
        }

//        cli_len = sizeof(cli_addr);
//        cfd = accept(sfd, (struct sockaddr *)&cli_addr, &cli_len);
//
        pid = fork();
        if (pid == 0) {
            close(sfd);
            while (1) {
                /*��ȡ�ͻ��˷�������*/
                len = read(cfd, buf, sizeof(SIZEBUF));
                if(len == 0){ //�ͻ��˹ر�
                    break;
                }
                write(STDOUT_FILENO, buf, len);

                /*����ͻ�������*/
                for (i = 0; i < len; i++) {
                    buf[i] = toupper(buf[i]);
                }
                /*���������ݻ�д���ͻ���*/
                write(cfd, buf, len);
            }
            close(cfd);
            return 0;
        } else if (pid > 0) {
            close(cfd);
        }  else
            exit(1);
    }
    /*��д�ļ�������*/
//    while(1){
//        /*��ȡ�Ѿ��������ֳɹ������󣬲���1��sfd; ��2��������, ��3���봫�����, ȫ����client�˵Ĳ���*/
//        cli_len = sizeof(cli_addr);                  //��ȡ�ͻ��˽ṹ���С
//        if((cfd = accept(sfd, (struct sockaddr *)&cli_addr, &cli_len)) == -1){  //�����ͻ�������, ������
//            perror("fail to bind socket");
//            exit(1);
//        }
//        pid = fork();
//        if(pid == -1){
//            perror("fail to fork");
//            exit(1);
//        }
//        if(pid == 0) { //�ӽ���
//            while (1) {
//                /*��ȡ�ͻ��˷�������*/
//                len = read(cfd, buf, sizeof(SIZEBUF));
//                if(len == 0){ //�ͻ��˹ر�
//                    break;
//                }
//                write(STDOUT_FILENO, buf, len);
//
//                /*����ͻ�������*/
//                for (i = 0; i < len; i++) {
//                    buf[i] = toupper(buf[i]);
//                }
//                /*���������ݻ�д���ͻ���*/
//                write(cfd, buf, len);
//            }
//            close(cfd);
//            return 0;
//        }
//    }
//    while(1);
    return 0;
}
