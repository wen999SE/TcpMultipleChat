#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H
#include "../../kernel_list.h"
#include <sys/socket.h>
#include <arpa/inet.h>

//使能聊天功能,不使用请注释掉define节省资源,使用到全局链表等资源
//enable chat feature ,if not use this feature, please comment out this define to save source,
//because this feature use golbal kernel list etc.
#define ENABLE_CHAT

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
int broadcastMsg(struct user *user_send, char *chat_buf);

struct user * init_list();
#endif // CHAT_SERVER_H
