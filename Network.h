#ifndef __NETWORK__
#define __NETWORK__

#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <pthread.h>

bool exitFlag = false;

int ListenLoop(int port);
int ConEstablish(void *);

#endif
