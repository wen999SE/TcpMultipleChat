#include <stdio.h>
#include <sys/socket.h>
#include "connectserver.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>



int ConnectServer_sockfd =0;

int main(int argc, char** argv)
{
    signal();

    if (argc != 3)
    {
        printf("parameter error！method：<ip><port>\n");
        exit(0);
    }

    //1.连接服务器
    ConnectServer_sockfd = ConnectServer(argv[1] ,argv[2]);

     //2.创建互斥锁,防止单一外设使用紊乱
    //create mutex to avoid to use single peripheral disordered
    pthread_mutex_init(&m, NULL);

    //3.创建子线程负责从服务端接收消息
    //the child thread use to receive massage from chat_server
    pthread_t chat_recv_tid;
    pthread_create(&chat_recv_tid, NULL, chat_recv_process, (void *)ConnectServer_sockfd);

    //4.主线程负责向服务器发送数据
    //main thread use to send data to the chat_server
    char chat_send_buf[100];
    int write_return = 0;
    while(1)
    {
        memset(chat_send_buf, 0 ,sizeof(char)*100);

        printf("\n请输入发送的消息:");
        if ( fgets(chat_send_buf, 100, stdin) == NULL )break;
        write_return = write(ConnectServer_sockfd, chat_send_buf, strlen(chat_send_buf));
        printf("%d\n",write_return);
        if (write_return != strlen(chat_send_buf) )
        {
            perror("error:");
        }
    }

    close(ConnectServer_sockfd);

    void *val;
    pthread_join(chat_recv_tid, &val);
    printf("exit program\n");
    return 0;
}



