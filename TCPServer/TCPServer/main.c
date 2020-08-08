#include <stdio.h>
#include <stdlib.h>
#include "chat_server.h"
#include <unistd.h>
#include <signal.h>

int server_socketfd = 0;

void cleanup(int sig __attribute__ ((unused)) )
{
    destroy_user_list();

    close(server_socketfd);
    printf("exit chat_server by ctrl+c\n");

    exit(0);

}

int main(int argc, char** argv)
{


    if (argc != 3)
    {
        printf("parameter error!methord:<ip><port>\n");
        exit(0);
    }

    //收到子进程结束后的信号
    //get child progress signal
    signal(SIGCHLD, cleanup);
    //得到ctrl+c的信号,进行程序退出处理
    //get ctrl+c signal , cleanup() the program before exit
    signal(SIGINT, cleanup);

    server_socketfd = Chat_Server_init(argv[1], argv[2], 5);

    while(1)
    {
        chat_recv_process(server_socketfd);
    }

    close(server_socketfd);
    //检查是否剔除了
    destroy_user_list();
    printf("exit chat_server\n");
    return 0;
}
