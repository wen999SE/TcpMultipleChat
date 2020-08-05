#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H
#include "../../kernel_list.h"
#include <sys/socket.h>
#include <arpa/inet.h>

struct user
{
    int ID;
    int connfd;
    struct sockaddr_in addr_in;

    struct list_head list;
};

extern struct user *user_head;


int Chat_Server_init(char *Server_ip ,char *Server_port, int recv_max);
int chat_recv_process(int socket_fd);
int chat_welcome(struct user *newone);


struct user * init_list();
#endif // CHAT_SERVER_H
