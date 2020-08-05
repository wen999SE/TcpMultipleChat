#include <stdio.h>
#include <sys/socket.h>
#include "connectserver.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int ConnectServer_sockfd =0;

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        printf("parameter error！method：<ip><port>\n");
        exit(0);
    }

    ConnectServer_sockfd = ConnectServer(argv[1] ,argv[2]);

    char chat_send_buf[100];
    while(1)
    {
        memset(chat_send_buf, 0 ,sizeof(char)*100);

        if ( fgets(chat_send_buf, 100, stdin) == NULL )break;

        write(ConnectServer_sockfd, chat_send_buf, strlen(chat_send_buf));
    }

    close(ConnectServer_sockfd);

    printf("exit program\n");
    return 0;
}
