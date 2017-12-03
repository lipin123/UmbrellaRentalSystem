﻿#include "SpotNetwork.h"
#include <string>
using namespace std;

#define DEBUG 1
#define DB_DEBUG 0

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
	string inq;
	bool gmerr = false;
	vector<string> re;
	inq = "UPDATE umbrella SET borrower_id=" + to_string(newSpotID) + ",slot_label=" + to_string(newSlotID) + " WHERE umbrella_id=" + to_string(umbrellaID) + ";";
	gmerr = dbExa.ExeSQL(inq);
	if (dbExa.affectedRows() > 0 && gmerr)
		return true;
	return false;
}

int SpotNetwork::GetUmbidByHash(string hash, int userID) // part 4
{
	string inq;
	vector<string> re;
	inq = "SELECT umbID FROM hashcode WHERE userID = '"+ to_string(userID) +"' AND hashCode='"+hash+"'";
	dbExa.ExeSQL(inq);
	re = dbExa.NextRow();
	if (re.size() > 0)
		return atoi(re[0].c_str());
	return -1;
}

int SpotNetwork::UpdateSpotAfterBorrow(int spotID, string umbStorage) // part 5
{
	string inq;
	vector<string> re;
	inq = "UPDATE rental_spot SET vacancy = '"+ umbStorage +"' WHERE rs_id = '"+to_string(spotID)+"'";
	dbExa.ExeSQL(inq);
	if (dbExa.affectedRows() > 0)
		return 0;
	return -1;
}

//DB function end

int SpotNetwork::ComunicateFunc(const int socket)
{
	switch (dataJson["command"].asInt())
	{
		case R2S_SendHash:
			return SendHash(socket);
		case R2S_RentConfirm:
			return RentalConfirm(socket);
		case R2S_ReturnUmbrellaCode:
			return ReturnUmbrellaCode(socket);
		case R2S_ReturnConfirm:
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

		if(DB_DEBUG)
		{
			/////////////////////////////////////////////////////
			//	DB part 1
			//	return 1 if it's not right RentalSpot ID
			if(CheckRentalSpotID(spotID))
			{
				sendFIN(socket);
				return 1;
			}
			////////////////////////////////////////////////////
		}
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

	if(DB_DEBUG)
	{
		////////////////////////////////////////////////////////////////
		//	DB part 4 
		//	userID와 hashCode를 가지고 올바른 건지 확인
		//	올바른 hashCode와 userID라면 우산 id를 반환
		//	틀리면 -1 반환
		GetUmbidByHash(hashCode, userID); //part 4
		////////////////////////////////////////////////////////////////
	}
	Json::Value sendData;

	sendData["command"] = S2R_VerifyCom;
	sendData["spotID"] = spotID;
	sendData["umbID"] = umbID;

	dataStreamWrite(socket, sendData);

	return 0;
}

int SpotNetwork::RentalConfirm(const int socket)
{
	int spotID = dataJson["spotID"].asInt();
	int status = dataJson["status"].asInt();
	string umbStorage = dataJson["umbStorage"].asString();

	if(DB_DEBUG)
	{
		//////////////////////////////////////////////////////////////////
		//	DB part 5
		//	spotID와 umbStorage를 이용해 대여 이후의 상황을 최신화
		//  이상이 없으면 0반환, 있으면 -1반환.
		//'剩余雨伞列表'没更新
		UpdateSpotAfterBorrow(spotID, umbStorage);
		//////////////////////////////////////////////////////////////////
	}
	return 1;
}

int SpotNetwork::ReturnUmbrellaCode(const int socket)
{
	int spotID = dataJson["spotID"].asInt();
	int umbID = dataJson["umbID"].asInt();
	int userID = 1234;
	Json::Value sendData;

	if(DB_DEBUG)
	{
		////////////////////////////////////////////////////////////////
		//  DB 파트 2
		//  umbID로 우산 번호 확인 후 대여중인지 확인
		//  만약 해당 우산번호가 없거나 대여중인 우산이 아니라면 -1 반환
		//  그게 아니라면 대여하고 있던 userID 반환
		if((userID = UmbrellaFindUsers(umbID)) == -1)
			return 1;
		//////////////////////////////////////////////////////////////
	}
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

	if(DB_DEBUG)
	{
		/////////////////////////////////////////////////////////////////
		// DB 파트 3
		// DB에 해당 ID의 우산에 '보관된 Spot ID'와 'Spot에 보관된 위치'
		// 를 수정
		// 해당 대여지점(Spot)이 보유한 우산 및 현재 보관 현황을 최신화
		UpdateUmbrellaLocation(umbID, spotID, returnPlace);//Please enter the parameters
		////////////////////////////////////////////////////////////////
	}

	if(status)
		cout<<spotID<<" have a problem for return"<<endl;

	return 1;
}
