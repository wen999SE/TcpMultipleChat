#include <stdio.h>
#include <stdlib.h>
#include "chat_server.h"
#include <unistd.h>

int server_socketfd = 0;
int main(int argc, char** argv)
{
    if (argc != 3)
    {
        printf("parameter error!methord:<ip><port>\n");
        exit(0);
    }


    server_socketfd = Chat_Server_init(argv[1], argv[2], 5);

    while(1)
    {
        chat_recv_process(server_socketfd);
    }

    close(server_socketfd);
    list_del(&user_head->list);
    free(user_head);
    return 0;
}
