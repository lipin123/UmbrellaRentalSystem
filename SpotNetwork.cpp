#include "SpotNetwork.h"


using namespace std;

int SpotNetwork::ComunicateFunc(const int socket)
{
	switch(dataJson["command"].asInt())
	{
		case R2S_SendHash:
			return ReturnUmbrellaCode(socket);
		case R2S_RentConfirm:
			return ReturnConfirm(socket);
		default:
	}
}

int SpotNetwork::Identification(const int socket)
{
	int spotID = dataJson["spotID"].asInt();

	if(dataJson["command"].asInt() == R2S_RentalIden)
	{
		Json::Value sendData;

		/////////////////////////////////////////////////////
		//	DB part
		//	return 1 if it's not right RentalSpot ID
		////////////////////////////////////////////////////
		
		sendData["ID"] = spotID;
		sendData["command"] = ACK;

		dataStreamWrite(socket, sendData);

		return 0;
	}else
		return 1;
}

int SpotNetwork::ReturnUmbrellaCode(const int socket)
{
	int spotID = dataJson["spotID"].asInt();
	int umbID = dataJson["umbID"].asInt();
	
	////////////////////////////////////////////////////////////////
	//  DB 파트
	//  umbID로 우산 번호 확인 후 대여중인지 확인
	//  만약 해당 우산번호가 없거나 대여중인 우산이 아니라면 -1 반환
	//  그게 아니라면 대여하고 있던 userID 반환
	//  ////////////////////////////////////////////////////////////
	
	sendData["spotID"] = spotID;
	sendData["command"] = S2R_VerifyUmbrellaCode;

	return 0;
}

int SpotNetwork::ReturnConfirm(const int socket)
{
	int spotID = dataJson["spotID"].asInt();
	int returnPlace = dataJson["spotID"].asInt();
	int status = dataJson["status"].asInt();
	string umbStorage = dataJson["umbStorage"].asString();

	/////////////////////////////////////////////////////////////////
	// DB에 해당 ID의 우산에 '보관된 Spot ID'와 'Spot에 보관된 위치'
	// 를 수정
	// 해당 대여지점(Spot)이 보유한 우산 및 현재 보관 현황을 최신화
	// //////////////////////////////////////////////////////////////
	
	return 1;
}
