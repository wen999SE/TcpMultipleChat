#include "connectserver.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

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
