#include "UserNetwork.h"

using namespace std;

int UserNetwork::ComunicateFunc(int socket, string data)
{
	if(stringToJson(data))
		return -1;


	switch(dataJson["command"].asInt())
	{
		case 0x00:	//최초 연결

			break;
		case 0x01:	//대여소 위치 요청

			break;
		case 0x03:	//대여소 선택

			break;
		case 0x05:	//우산 선택

			break;
	}

}
