#ifndef CONNECTSERVER_H
#define CONNECTSERVER_H
#include <pthread.h>
extern pthread_mutex_t m;
extern int ConnectServer_sockfd;

int ConnectServer(char* ip ,char* port);
void *chat_recv_pthread(void *arg);
void chat_recv_pthread_cleanup(void *arg);

#endif // CONNECTSERVER_H
