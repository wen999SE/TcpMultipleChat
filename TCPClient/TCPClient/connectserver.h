#ifndef CONNECTSERVER_H
#define CONNECTSERVER_H
#include <pthread.h>
extern pthread_mutex_t m;


int ConnectServer(char* ip ,char* port);
void *chat_recv_process(void *arg);

#endif // CONNECTSERVER_H
