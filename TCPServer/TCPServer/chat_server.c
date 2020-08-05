#include "chat_server.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../kernel_list.h"
#include <sys/types.h>
#include <unistd.h>

struct user *user_head;
struct list_head *pos;//临时变量 temp variate
struct user *p;//临时变量 temp variate

int Chat_Server_init(char *Server_ip, char *Server_port, int recv_max)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        perror("create server socket failed!\n");
        exit(0);
    }


    memset(&addr, 0, len);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(Server_ip);
    addr.sin_port = htons(atoi(Server_port));


    if(bind(socket_fd, (struct sockaddr *)&addr, len) == -1)
    {
        perror("bind socket failed!\n");
        exit(0);
    }

    //listen socket and set max connect number equal 5
    if (listen(socket_fd, recv_max) != 0)
    {
        perror("Failed to listen socket");
        exit(0);
    }

    //initial user list
    user_head = calloc(1, sizeof(struct user) );
    if (user_head != NULL)
    {
        INIT_LIST_HEAD(&user_head->list);
    }

    return socket_fd;

}

int chat_welcome(struct user *newone)
{
    struct list_head *pos;
    list_for_each(pos, &user_head->list)
    {
        p = list_entry(pos, struct user, list);

        char buf[50];
        memset(buf, 0, sizeof(char)*50);
        //send ID to new user
        if (p->ID == newone->ID)
            snprintf(buf, 50, "你的ID是：%d\n", newone->ID);
        //notify all users that is a new user is login
        else
            snprintf(buf, 50, "用户:%d上线了!\n",newone->ID);

        write(p->connfd, buf, strlen(buf));

    }
}

fd_set rset;//read ready socket flag
fd_set wset;//write ready socket flag
fd_set eset;//error ready socket flag
int ID = 0;



int chat_recv_process(int socket_fd)
{

    //1.reset rset wset eset
    FD_ZERO(&rset);
    FD_ZERO(&wset);
    FD_ZERO(&eset);

    int maxfd = socket_fd;
    //bind socket_fd to rset ,when new connect coming then rset will set
    //把socket_fd 置入 rset集合,当有新的连接到来时,相应绑定的rset位会set
    FD_SET(socket_fd, &rset);

    list_for_each(pos,user_head->list)
    {
        p = list_entry(pos, struct user, list);
        FD_SET(p->connfd, &rset);
        maxfd = (maxfd > p->connfd) ? maxfd : p->connfd;
    }

    //2.
    select(maxfd+1, &rset, &wset, &eset, NULL);



    //a.judge whether there is a new connection
    socklen_t len = 0;
    if (FD_ISSET(socket_fd, &rset))
    {
        //get a new connection,create a new user list node
        struct user *newone = calloc(1, sizeof(struct user));
        len = sizeof(newone->addr_in);
        newone->connfd = accept(socket_fd, (struct sockaddr *)&newone->addr_in, &len);
        if (newone->connfd == -1)
        {
            perror("Failed to accept socket");
            exit(0);
        }

        //put the new user list node to tail
        list_add_tail(&newone->list, &user_head->list);

        //distribute user id
        newone->ID = ID;
        ID++;

        //notify all users that a new user has joined
        //send welcome and ID to new user
        chat_welcome(newone);

        //chat_server show message to screen
        char chat_buf[100];
        memset(chat_buf, 0, sizeof(char)*100);
        int m = read(newone->connfd, chat_buf ,100);
        if (m>0)
        printf("[%s:%hu]:%s", inet_ntoa(newone->addr_in.sin_addr), ntohs(newone->addr_in.sin_port), chat_buf);
    }

    //b.judge whether there is user send massage
    //and judge if someuser had offline
    list_for_each_safe()
    return 0;
}
