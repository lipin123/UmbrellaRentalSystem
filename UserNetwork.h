#ifndef __USER_NETWORK__
#define __USER_NETWORK__

#include <iostream>
#include <string>
#include "Server_Network_format.h"
#include "Network.h"
#include "MyDB.h"

class UserNetwork : public Network
{
	private:
		MyDB dbExa;

	public:
		UserNetwork()
		{
			dbExa.InitDB("localhos", "root", "1234", "umbrella", 3306);
		}

	private:
		//유저 클라이언트의 데이터 패킷을 처리하는 함수
		//
		//socket : 클라이언트 소켓 fd
		int ComunicateFunc(const int socket);

		int Identification(const int socket);

		int SpotRequest(const int socket);

		int SelectSpot(const int socket);

		int SelectUmbrella(const int socket);


		//////////////////////////////////////////////////////////////
		//DB part
		int CheckUserID(int userID);//part 1
		coordinates GetSpotCdnt(int SportID);//part 2
		vector<rentalSpot> GetRentalspotInfo(int RentalspotID);//part 3
		bool AdHashCode(int userID,int spotID,int umbNum,string hashCode); //part 4
};

#endif
