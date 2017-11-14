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

#define DEBUG true	//true일때는 에코서버가 됨
#define BUFFER_LEN 4096	//4kB

//서버 종료 플래그
bool exitFlag = false;

//클라이언트 패킷을 처리하는 함수
//
//socket : 클라이언트 소켓
//data : 클라이언트로 받은 패킷
//
//0		-> 연결 유지
//1		-> 연결 종료
typedef int (*ComunicateFunction)(int socket, std::string data);

//listen 루프를 도는 함수
//
//port : 해당 루프가 데이터를 받을 port번호
//func : 클라이언트에게 패킷을 받았을때 처리할 함수 포인터(클라이언트 소켓, string 데이터)
//
//0		-> 정상종료
//-1	-> 비정상 종료(에러 메세지 출력)
int ListenLoop(int port, ComunicateFunction func);

#endif
