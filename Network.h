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

#define DEBUG true	//true일때는 에코서버가 됨
#define BUFFER_MAX_LEN 4096	//4kB

//서버 종료 플래그
bool exitFlag = false;


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
		virtual int ComunicateFunc(int socket, std::string data) = 0;

		//클라이언트로 부터 데이터를 받는 함수
		//
		//socket : 클라이언트 소켓
		//buffer : 버퍼 포인터
		//
		//0		-> 성공
		//1		-> EOF 탐지
		//-1	-> 버퍼 크기 오버
		int dataStreamRead(int socket, char * buffer);

		//string을 json형태로 바꿔서 저장해주는 함수
		//
		//data : string데이터
		//
		//0 	-> 성공
		//1		-> 입력받은 스트링이 json데이터가 아님
		int stringToJson(std::string data);

	public:
		//listen 루프를 도는 함수
		//
		//port : 해당 루프가 데이터를 받을 port번호
		//
		//0		-> 정상종료
		//-1	-> 비정상 종료(에러 메세지 출력)
		int ListenLoop(int port);
};
#endif
