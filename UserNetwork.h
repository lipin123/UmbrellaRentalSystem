#ifndef __USER_NETWORK__
#define __USER_NETWORK__

#include <iostream>
#include <string>
#include "Server_Network_format.h"
#include "Network.h"

class UserNetwork : public Network
{
	

	private:
	//유저 클라이언트의 데이터 패킷을 처리하는 함수
	//
	//socket : 클라이언트 소켓 fd
	//data : 클라이언트의 데이터
	int ComunicateFunc(int socket, std::string data);
};

#endif
