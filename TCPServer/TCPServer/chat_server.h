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

//chat_server
int Chat_Server_init(char *Server_ip ,char *Server_port, int recv_max);
int chat_recv_process(int socket_fd);
int chat_welcome(struct user *newone);
int broadcastMsg(struct user *user_send, char *chat_buf);
int private_chat(struct user *private_send_user, int private_recv_user_ID ,char *chat_private_buf);
int private_recv_file(struct user *private_send, char *filename);

//user_list
void destroy_user_list(void);
void show_user_list(void);


#endif // CHAT_SERVER_H
