#include "UserNetwork.h"
#include "sha256.h"
#include <ctime>
#include <cstdlib>
using namespace std;

#define DEBUG_USER 1

//DB part begin
MyDB dbExa;
dbExa.InitDB("localhost", "root", "1234", "umbrellarental", 3306);

int UserNetwork::CheckUserID(int userID) //part 1
{
	vector<user> usRes;
	usRes=dbExa.UniSearch(user_id, to_string(userID));
	if (usRes.size() <= 0)
		return 1;
	return 0;
}

coordinates UserNetwork::GetSportCdnt(int SportID) //part 2
{
	coordinates re;
	vector<rentalSpot> rsRes;
	rsRes = dbExa.UniSearch(rs_id, to_string(SportID));
	if (rsRes.size() > 0)
	{
		re.lat = rsRes[0].lat;
		re.lng = rsRes[0].lng;
	}
	return re;
}

vector<rentalSpot> UserNetwork::GetRentalspotInfo(int RentalspotID) //part 3
{
	vector<rentalSpot> rs;
	rs = dbExa.UniSearch(rs_id, to_string(RentalspotID));
	return rs;
}

bool UserNetwork::AdHashCode(int userID, int spotID, int umbNum, string hashCode) //part 4
{
	string inq;
	inq = "call Uni_insert(\"hashcode\",\"'"+to_string(userID)+"','"+ to_string(spotID) +"','"+ to_string(umbNum) +"','"+ hashCode +"'\")";
	return dbExa.ExeSQL(inq);
}

//DB part end


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
		default:
			if(DEBUG_USER)
				cout<<"just out"<<endl;
	}

}

int UserNetwork::Identification(const int socket)
{
	int userID = dataJson["userID"].asInt();

	if(dataJson["command"].asInt() == U2S_UserIden)
	{
		Json::Value sendData;
		//////////////////////////////////////////
		//  DB part1
		//	유저 ID DB에서 체크
		//	만약 아니라면 1 반환
		CheckUserID(userID); //part1
		//////////////////////////////////////////

		sendData["ID"] = userID;
		sendData["command"] = ACK;

		dataStreamWrite(socket, sendData);

		if(DEBUG)
			cout<<"Identified"<<userID<<endl;
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
	// DB part2
	// 우산 대여소 위치 데이터들 받기
	GetSportCdnt(int RentalspotID); //대여소 ID를 입력하셈.
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
	// DB part3
	// 대여지점 정보 받기
	GetRentalspotInfo(int RentalspotID) //대여소 ID를 입력하셈.
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
	// DB part4
	// 해쉬 코드와 해야할 명령들 DB에 저장

	AdHashCode(int userID, int spotID, int umbNum, hashCode); //userID,spotID,umbNum를 입력하셈.
	///////////////////////////////////////////////////
	
	sendData["hashCode"] = hashCode;

	dataStreamWrite(socket, sendData);

	return 0;
}


	
