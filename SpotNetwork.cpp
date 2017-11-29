#include "SpotNetwork.h"
#include <string>
using namespace std;

#define DEBUG 1


int SpotNetwork::CheckRentalSpotID(int spotID)//part1
{
	rentalSpotCol rsCol = rs_id;
	vector<rentalSpot> rsRet;
	rsRet = dbExa.UniSearch(rsCol, to_string(spotID));
	if (rsRet.size() <= 0)
		return 1;
}

int SpotNetwork::UmbrellaFindUsers(int umbrellaid)//part2
{
	umbrellaCol umCol = umbrella_id;
	vector< umbrellas> umRes;
	umRes = dbExa.UniSearch(umCol, to_string(umbrellaid));
	if (umRes.size() <= 0 || umRes[0].umbrellaStatus == 1)
		return -1;
	return umRes[0].locationID;
}

bool SpotNetwork::UpdateUmbrellaLocation(int umbrellaID, int newSpotID, int newSlotID)//part3
{
	bool update1 = false, update2 = false;
	update1 = dbExa.UniUpdate(umbrella_id, to_string(umbrellaID), borrower_id, to_string(newSpotID));
	update2 = dbExa.UniUpdate(umbrella_id, to_string(umbrellaID), slot_label, to_string(newSlotID));
	if (update1&&update2)
		return true;
	return false;
}
//DB function end

int SpotNetwork::ComunicateFunc(const int socket)
{
	switch (dataJson["command"].asInt())
	{
	case R2S_SendHash:
		return ReturnUmbrellaCode(socket);
	case R2S_RentConfirm:
		return ReturnConfirm(socket);
	default:
		break;
	}
}

int SpotNetwork::Identification(const int socket)
{
	int spotID = dataJson["spotID"].asInt();

	if (dataJson["command"].asInt() == R2S_RentalIden)
	{
		Json::Value sendData;

		/////////////////////////////////////////////////////
		//	DB part 1
		//	return 1 if it's not right RentalSpot ID
		if(CheckRentalSpotID(spotID))
			return 1;
		////////////////////////////////////////////////////

		sendData["ID"] = spotID;
		sendData["command"] = ACK;

		dataStreamWrite(socket, sendData);

		return 0;
	}
	else
		return 1;
}

int SpotNetwork::SendHash(const int socket)
{
	int spotID = dataJson["spotID"].asInt();
	int userID = dataJson["userID"].asInt();
	string hashCode = dataJson["hashCode"].asString();
	int umbID = 0;

	////////////////////////////////////////////////////////////////
	//	DB part
	//	userID와 hashCode를 가지고 올바른 건지 확인
	//	올바른 hashCode와 userID라면 우산 id를 반환
	//	틀리면 -1 반환
	////////////////////////////////////////////////////////////////
	
	Json::Value sendData;

	sendData["command"] = S2R_VerifyCom;
	sendData["spotID"] = spotID;
	sendData["umbID"] = umbID;

	dataStreamWrite(socket sendData);

	return 0;
}

int SpotNetwork::RentalConfirm(const int socket)
{
	int spotID = dataJson["spotID"].asInt();
	int status = dataJson["status"].asInt();
	string umbStorage = dataJson["umbStorage"].asInt();

	//////////////////////////////////////////////////////////////////
	//	DB part
	//	spotID와 umbStorage를 이용해 대여 이후의 상황을 최신화
	//  이상이 없으면 0반환
	//////////////////////////////////////////////////////////////////
	
	return 1;
}

int SpotNetwork::ReturnUmbrellaCode(const int socket)
{
	int spotID = dataJson["spotID"].asInt();
	int umbID = dataJson["umbID"].asInt();
	int userID = 1234;
	Json::Value sendData;

	////////////////////////////////////////////////////////////////
	//  DB 파트 2
	//  umbID로 우산 번호 확인 후 대여중인지 확인
	//  만약 해당 우산번호가 없거나 대여중인 우산이 아니라면 -1 반환
	//  그게 아니라면 대여하고 있던 userID 반환
	if((userID = UmbrellaFindUsers(umbID)) == -1)
		return 1;
	//////////////////////////////////////////////////////////////

	sendData["spotID"] = spotID;
	sendData["command"] = S2R_VerifyUmbrellaCode;
	sendData["userID"] = userID;
	sendData["umbID"] = umbID;

	dataStreamWrite(socket, sendData);

	return 0;
}

int SpotNetwork::ReturnConfirm(const int socket)
{
	int spotID = dataJson["spotID"].asInt();
	int returnPlace = dataJson["returnPlace"].asInt();
	int status = dataJson["status"].asInt();
	int umbID = dataJson["umbID"].asInt();
	string umbStorage = dataJson["umbStorage"].asString();

	/////////////////////////////////////////////////////////////////
	// DB 파트 3
	// DB에 해당 ID의 우산에 '보관된 Spot ID'와 'Spot에 보관된 위치'
	// 를 수정
	// 해당 대여지점(Spot)이 보유한 우산 및 현재 보관 현황을 최신화
	UpdateUmbrellaLocation(umbID, spotID, returnPlace);//Please enter the parameters
	////////////////////////////////////////////////////////////////

	if(status)
		cout<<spotID<<" have a problem for return"<<endl;


	return 1;
}
