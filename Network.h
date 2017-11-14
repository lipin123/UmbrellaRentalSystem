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

typedef int (*ComunicateFunction)(std::string);

int ListenLoop(int port, ComunicateFunction func);
int ConEstablish(void *);

#endif
