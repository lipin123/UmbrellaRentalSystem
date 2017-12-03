#include "UserNetwork.h"
#include "sha256.h"
#include <ctime>
#include <cstdlib>
#include <cmath>
using namespace std;

#define DEBUG_USER 1
#define DB_DEBUG 0


int UserNetwork::CheckUserID(int userID) //part 1
{
	vector<user> usRes;
	usRes=dbExa.UniSearch(user_id, to_string(userID));
	if (usRes.size() <= 0)
		return 1;
	return 0;
}

vector<vector<double>> UserNetwork::GetSpotCdnt(double lat, double lng, int range) //part2
{
	vector<vector<double>> re;
	vector<rentalSpot> rsRes;
	rsRes = dbExa.SearchSpot(lat, lng, range);
	if (rsRes.size() > 0)
	{
		for (int i = 0; i < rsRes.size(); i++)
		{
			vector<double> tem;
			tem.push_back(rsRes[i].rsID);
			tem.push_back(rsRes[i].lat);
			tem.push_back(rsRes[i].lng);
			tem.push_back(rsRes[i].umbrellaID.size());
			re.push_back(tem);
		}
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
	vector<string> re;
	inq = "SELECT umbrella_id FROM umbrella WHERE borrower_id = " + to_string(spotID) + " AND slot_label = " + to_string(umbNum) + ";";
	dbExa.ExeSQL(inq);
	re = dbExa.NextRow();
	if (re.size() > 0)
	{
		inq = "INSERT INTO hashcode VALUES('" + to_string(userID) + "','" + re[0] + "','" + hashCode + "');";
		return dbExa.ExeSQL(inq) && (dbExa.affectedRows()>0);
	}
	return false;
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
			return 0;
	}

}

int UserNetwork::Identification(const int socket)
{
	int userID = dataJson["userID"].asInt();

	if(dataJson["command"].asInt() == U2S_UserIden)
	{
		Json::Value sendData;

		if(DB_DEBUG)
		{
			//////////////////////////////////////////
			//  DB part1
			//	유저 ID DB에서 체크
			//	만약 아니라면 1 반환
			if(CheckUserID(userID))
				return 1; //part1
			//////////////////////////////////////////
		}
		sendData["ID"] = userID;
		sendData["command"] = ACK;

		dataStreamWrite(socket, sendData);

		if(DEBUG_USER)
			cout<<"Identified"<<userID<<endl;
		return 0;
	}else
		return 0;	//Should Modify later!
}

int UserNetwork::SpotRequest(const int socket)
{
	Json::Value sendData;
	Json::Value SpotLocation;
	int locationNum = 0;
	int spotID = dataJson["spotID"].asInt();

	double E = dataJson["E"].asDouble();
	double N = dataJson["N"].asDouble();
	double width = dataJson["width"].asDouble();
	double height = dataJson["height"].asDouble();


	vector<vector<double>> cord;


	sendData["command"] = S2U_RentalPos;
	sendData["userID"] = dataJson["userID"].asInt();
	if(DB_DEBUG)
	{
		////////////////////////////////////////////////////
		// DB part2
		// 우산 대여소 위치 데이터, SpotID, 우산수 데이터들 받기
		cord=GetSpotCdnt(N, E, sqrt(pow(width / 2, 2) + pow(height / 2, 2)));
		////////////////////////////////////////////////////
	}	
	SpotLocation["E"] = E;			//for DEBUG - change later
	SpotLocation["N"] = N;			//for DEBUG - change later
	SpotLocation["spotID"] = 1;		//DB에서 받아야하는 데이터
	SpotLocation["numOfUmb"] = 1;	//DB에서 받아야하는 데이터
	locationNum++;
	sendData["SpotLocation"].append(SpotLocation);
	sendData["numOfSpot"] = locationNum;

	dataStreamWrite(socket, sendData);

	return 0;
}

int UserNetwork::SelectSpot(const int socket)
{
	Json::Value sendData;
	int spotID;
	rentalSpot spot;

	sendData["command"] = S2U_SpotInfo;
	sendData["userID"] = dataJson["userID"].asInt();
	sendData["spotID"] = spotID = dataJson["spotID"].asInt();
	sendData["x"] = 1;
	sendData["y"] = 1;


	if(DB_DEBUG)
	{
		///////////////////////////////////////////////
		// DB part3
		// 대여지점 정보 받기
		spot = GetRentalspotInfo(spotID)[0]; //대여소 ID를 입력하셈.
		///////////////////////////////////////////////
	}

	string umbStorage = "1";

	sendData["x"] = spot.structure.col;
	sendData["y"] = spot.structure.row;

	for(int i = 0; i<spot.vacancy.size();i++)
	{
		umbStorage+=spot.vacancy[i];
	}

	sendData["umbStorage"] = umbStorage;

	dataStreamWrite(socket, sendData);

	return 0;
}

int UserNetwork::SelectUmbrella(const int socket)
{
	Json::Value sendData;
	int umbNum;
	int userID;
	int spotID;
	string source;

	srand(time(NULL));

	sendData["command"] = S2U_UmbHash;
	sendData["userID"] = userID = dataJson["userID"].asInt();
	sendData["spotID"] = spotID = dataJson["spotID"].asInt();

	umbNum = dataJson["umbNum"].asInt();

	//해쉬코드를 생성하기 위한 string 생성
	source = sendData["userID"].asString();
	source += sendData["spotID"].asString();
	source += dataJson["umbNum"].asString();
	source += std::to_string(rand());

	string hashCode = sha256(source);

	if(DB_DEBUG)
	{
		///////////////////////////////////////////////////
		// DB part4
		// 해쉬 코드와 해야할 명령들 DB에 저장
		AdHashCode(userID, spotID, umbNum, hashCode);
		///////////////////////////////////////////////////
	}

	sendData["hashCode"] = hashCode;

	dataStreamWrite(socket, sendData);

	return 1;
}
