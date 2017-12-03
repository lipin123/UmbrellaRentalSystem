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

#ifndef __JSON__
#define __JSON__
#include "json/json.h"
#endif

#define BUFFER_MAX_LEN 4096	//4kB

//서버 종료 플래그
static bool exitFlag = false;


class Network
{
	protected:
		Json::Value dataJson;

	protected:
		//클라이언트 패킷을 처리하는 함수
		//
		//socket : 클라이언트 소켓
		//data : 클라이언트로 받은 패킷
		//
		//0		-> 연결 유지
		//1		-> 연결 종료
		//-1	-> 에러 발생
		virtual int ComunicateFunc(const int socket) = 0;

		//ID 확인
		//
		//socket : 클라이언트 소켓 fd
		//
		//0		-> 유저 맞음
		//1		-> 유저 아님, 즉시 해당 소켓과의 연결 종료
		virtual int Identification(const int socket) = 0;
		
		//클라이언트로 부터 데이터를 받아서 제이슨 형태로 저장
		//
		//socket : 클라이언트 소켓
		//
		//0		-> 성공
		//1		-> EOF 탐지
		//-1	-> 버퍼 크기 오버
		int dataStreamRead(const int socket);

		//데이터를 클라이언트로 제이슨 형태로 보냄
		//
		//socket : 클라이언트 소켓
		//data : 보내야할 제이슨 데이터
		//
		//0		-> 성공
		int dataStreamWrite(const int socket, Json::Value &data);

		//string을 json형태로 바꿔서 저장해주는 함수
		//
		//data : string데이터
		//
		//0 	-> 성공
		//1		-> 입력받은 스트링이 json데이터가 아님
		int stringToJson(const std::string &data);

		//연결 종료 신호를 보내는 함수
		//
		//socket : 클라이언트 소켓
		int sendFIN(const int socket);

	public:
		//listen 루프를 도는 함수
		//
		//port : 해당 루프가 데이터를 받을 port번호
		//
		//0		-> 정상종료
		//-1	-> 비정상 종료(에러 메세지 출력)
		int ListenLoop(const int port);
};
#endif
