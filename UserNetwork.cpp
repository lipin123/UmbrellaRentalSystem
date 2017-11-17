#include "UserNetwork.h"
#include "sha256.h"
#include <ctime>
#include <cstdlib>
using namespace std;



int UserNetwork::ComunicateFunc(const int socket)
{
	
	switch(dataJson["command"].asInt())
	{
		case U2S_ReqRentalPos:	//대여소 위치 요청
			return SpotRequest(socket);
		case U2S_SelSpot:	//대여소 선택
			return SelectSpot(socket);
		case U2S_SelUmb:	//우산 선택
			return SelectUmbrella(socket);
	}

}

int UserNetwork::Identification(const int socket)
{
	int userID = dataJson["ID"].asInt();

	if(dataJson["command"].asInt() == U2S_UserIden)
	{
		Json::Value sendData;
		//////////////////////////////////////////
		//	유저 ID DB에서 체크
		//	만약 아니라면 1 반환
		//////////////////////////////////////////

		sendData["userID"] = userID;
		sendData["command"] = ACK;

		dataStreamWrite(socket, sendData);

		return 0;
	}else
		return 1;
}

int UserNetwork::SpotRequest(const int socket)
{
	Json::Value sendData;
	Json::Value SpotLocation;
	int locationNum = 0;

	sendData["command"] = S2U_RentalPos;
	sendData["userID"] = dataJson["userID"].asInt();

	////////////////////////////////////////////////////
	// 우산 위치 데이터들 받기
	////////////////////////////////////////////////////

	sendData["numOfSpot"] = locationNum;
	sendData["SpotLocation"] = SpotLocation;

	dataStreamWrite(socket, sendData);

	return 0;
}

int UserNetwork::SelectSpot(const int socket)
{
	Json::Value sendData;

	sendData["command"] = S2U_SpotInfo;
	sendData["userID"] = dataJson["userID"].asInt();
	sendData["spotID"] = dataJson["spotID"].asInt();

	///////////////////////////////////////////////
	// 대여지점 정보 받기
	///////////////////////////////////////////////

	dataStreamWrite(socket, sendData);

	return 0;
}

int UserNetwork::SelectUmbrella(const int socket)
{
	Json::Value sendData;
	int umbNum;
	string source;
	
	srand(time(NULL));

	sendData["command"] = S2U_UmbHash;
	sendData["userID"] = dataJson["userID"].asInt();
	sendData["spotID"] = dataJson["spotID"].asInt();

	//해쉬코드를 생성하기 위한 string 생성
	source = sendData["userID"].asString();
	source += sendData["spotID"].asString();
	source += dataJson["umbNum"].asString();
	source += std::to_string(rand());

	string hashCode = sha256(source);

	///////////////////////////////////////////////////
	// 해쉬 코드와 해야할 명령들 DB에 저장
	///////////////////////////////////////////////////
	
	sendData["hashCode"] = hashCode;

	dataStreamWrite(socket, sendData);

	return 0;
}
	
