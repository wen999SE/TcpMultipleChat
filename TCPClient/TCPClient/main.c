#include <stdio.h>
#include <sys/socket.h>
#include "connectserver.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>




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

        
        if ( strstr(chat_send_buf, "+") != NULL)//上传文件
        {
            long private_file_length = 0;
            char chat_temp_buf[1024], private_filename[50];
            FILE *fp;

            printf("[%s:%d]上传文件\r\n",  __FUNCTION__, __LINE__);
                
            //提取文件名和ID  
            //fetch ID of file  
            sscanf(chat_send_buf,"+ %s", private_filename);
            errno = 0;
            //提取文件长度
            //fetch ID of file  
            struct stat private_filename_info;
            stat(private_filename , &private_filename_info);
            if (errno == ENOENT) 
            {
                perror("文件不存在!");
                continue;
            }
            private_file_length = private_filename_info.st_size;

            memset(chat_temp_buf, 0, sizeof(char)*1024 );
            snprintf(chat_temp_buf, 1024,"+%ld %s", private_file_length, private_filename);

            //发送传输文件命令给服务器
            //send translate file command to server 
            write_return = write(ConnectServer_sockfd, chat_temp_buf, strlen(chat_temp_buf));
            

            int file_temp_length = 0;
            int file_temp_total = 0;
            fp = fopen(private_filename, "r");
            rewind(fp);
            if ( fp == NULL)
            {
                perror("faild to open!");
                continue;
            }
            while ( private_file_length > 0 )
            {
                memset(chat_temp_buf, 0, sizeof(char)*1024 );
                file_temp_length = fread(chat_temp_buf, 1, 1024, fp);
                file_temp_length = write(ConnectServer_sockfd, chat_temp_buf, file_temp_length );
                private_file_length -= file_temp_length;
                file_temp_total += file_temp_length;
                printf("已经传输了%d[字节]\r\n", file_temp_total);
            }
            fclose(fp);
            printf("发送完成!\n");
            
            continue;
        }
        //正常发送消息
        write_return = write(ConnectServer_sockfd, chat_send_buf, strlen(chat_send_buf));
    }


    close(ConnectServer_sockfd);

    void *val;
    pthread_join(chat_recv_tid, &val);
    printf("exit program\n");
    return 0;
}



