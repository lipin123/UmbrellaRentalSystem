#include "SpotNetwork.h";

#include <string>;

using namespace std;

//DB function begin
MyDB dbExa;
dbExa.InitDB("localhost", "root", "1234", "umbrellarental", 3306);

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
		CheckRentalSpotID(spotID);//part 1
		////////////////////////////////////////////////////

		sendData["ID"] = spotID;
		sendData["command"] = ACK;

		dataStreamWrite(socket, sendData);

		return 0;
	}
	else
		return 1;
}

int SpotNetwork::ReturnUmbrellaCode(const int socket)
{
	int spotID = dataJson["spotID"].asInt();
	int umbID = dataJson["umbID"].asInt();

	////////////////////////////////////////////////////////////////
	//  DB 파트 2
	//  umbID로 우산 번호 확인 후 대여중인지 확인
	//  만약 해당 우산번호가 없거나 대여중인 우산이 아니라면 -1 반환
	//  그게 아니라면 대여하고 있던 userID 반환
	UmbrellaFindUsers(umbID);//part 2
	//////////////////////////////////////////////////////////////

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
	// DB 파트 3
	// DB에 해당 ID의 우산에 '보관된 Spot ID'와 'Spot에 보관된 위치'
	// 를 수정
	// 해당 대여지점(Spot)이 보유한 우산 및 현재 보관 현황을 최신화
	UpdateUmbrellaLocation(int umbrellaID, int newSpotID, int newSlotID)//Please enter the parameters
	////////////////////////////////////////////////////////////////

		return 1;
}
