#include "connectserver.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>


pthread_mutex_t m;

int ConnectServer_sockfd =0;

int ConnectServer(char* ip ,char* port)
{
    //create tcp socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        perror("Failed to create UDP socket!");
        exit(0);
    }

    //define ip and prot
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    memset(&addr, 0, len);

    addr.sin_family = AF_INET;//指定协议族，选择ipv4
    addr.sin_addr.s_addr = inet_addr(ip);// ip address
    addr.sin_port = htons(atoi(port));//the port of connection

    //connet to the server and bind socket

    int connfd = connect(sockfd, (struct sockaddr *)&addr, len);
    if (connfd != 0)
    {
        perror("Failed to create TCP connect!");
        exit(0);
    }


    return sockfd;
}
void chat_recv_pthread_cleanup(void *arg)
{
    pthread_mutex_unlock(&m);

    //shutdown(ConnectServer_sockfd, 1);
    close(ConnectServer_sockfd);

    printf("退出线程:chat_recv_pthread_exit\n");

}
void *chat_recv_pthread(void *arg)
{
    int read_return = 0;
    char chat_recv_buf[100];

    //收到cancel信号是异步退出线程
    //when get signal - cancel, then exit pthread-chat_recv_pthread asynchronous
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    pthread_cleanup_push(chat_recv_pthread_cleanup, NULL);

    while(1)
    {
        memset(chat_recv_buf, 0, sizeof(char)*100);
        read_return = read(*( (int *)arg ) , chat_recv_buf, 100);

        if(read_return > 0)
        {
            pthread_mutex_lock(&m);
            printf("\n\n%s", chat_recv_buf);
            printf("请输入发送的消息:");
            fflush(stdout);
            pthread_mutex_unlock(&m);
        }
        else
        {
            printf("%d\r\n",read_return);
            break;
        }

    }

    close(ConnectServer_sockfd);
    pthread_mutex_lock(&m);
    printf("服务器关闭,关闭套接字,退出程序\n");
    pthread_mutex_unlock(&m);
    pthread_cleanup_pop(0);
    exit(0);
}
