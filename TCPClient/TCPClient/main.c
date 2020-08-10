#include <stdio.h>
#include <sys/socket.h>
#include "connectserver.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>




pthread_t chat_recv_tid;


void Cleanup(int sig __attribute__ ((unused)) )
{
    void *val;

    //1.关闭ConnectServer_sockfd的写入通道,使得server read的返回值为0,然后关闭和client的连接
    //2.client线程读取到read 0后break循环,关闭ConnectServer_sockfd的连接,退出线程
    // 1.close write channel of ConnectServer_sockfd ,then server read the massage is 0,then close
    // the connect with client.
    //2.when pthread-client read 0,bread while ,close the connect with server , pthread_exit();

    shutdown(ConnectServer_sockfd, 1);
    //close(ConnectServer_sockfd);

    //尽量不要直接结束线程
    //pthread_cancel(chat_recv_tid);

    //回收线程资源
    pthread_join(chat_recv_tid, &val);

    printf("exit program by ctrl+c\n");
}


int main(int argc, char** argv)
{
    if (argc != 3)
    {
        printf("parameter error！method：<ip><port>\n");
        exit(0);
    }
    struct sigaction signal_act;

    signal_act.sa_handler = Cleanup;
    sigemptyset(&signal_act.sa_mask);
    signal_act.sa_flags = 0;

    //当终端键入ctrl+c时候发出该信号
    //when terminate from terminal by Interrupt(ctrl+c) from keyboard will get this signal

    sigaction(SIGINT, &signal_act, NULL);
    //signal(SIGINT, Cleanup);

    //当子进程结束的时候,父进程会收到这个信号
    // when child process is terminate,parent process will get this signal-SIGCHLD
    sigaction(SIGCHLD, &signal_act, NULL);
    //signal(SIGCHLD, Cleanup);

    //1.连接服务器
    ConnectServer_sockfd = ConnectServer(argv[1] ,argv[2]);

     //2.创建互斥锁,防止单一外设使用紊乱
    //create mutex to avoid to use single peripheral disordered
    pthread_mutex_init(&m, NULL);

    //3.创建子线程负责从服务端接收消息
    //the child thread use to receive massage from chat_server

    pthread_create(&chat_recv_tid, NULL, chat_recv_pthread, (void *)&ConnectServer_sockfd);

    //4.主线程负责向服务器发送数据
    //main thread use to send data to the chat_server
    char chat_send_buf[100];
    int write_return = 0;

    while(1)
    {
        memset(chat_send_buf, 0 ,sizeof(char)*100);

        pthread_mutex_lock(&m);
        printf("\n请输入发送的消息:");
        pthread_mutex_unlock(&m);
        if ( fgets(chat_send_buf, 100, stdin) == NULL )break;
        write_return = write(ConnectServer_sockfd, chat_send_buf, strlen(chat_send_buf));
    }


    close(ConnectServer_sockfd);

    void *val;
    pthread_join(chat_recv_tid, &val);
    printf("exit program\n");
    return 0;
}



