#ifndef __SPOT_NETWORK__
#define __SPOT_NETWORK__

#include "Server_Network_format.h"
#include "Network.h"
#include <iostream>
#include "MyDB.h"

class SpotNetwork : public Network
{
	private:
		MyDB dbExa;

	public:
		SpotNetwork()
		{
			dbExa.InitDB("localhos", "root", "1234", "umbrella", 3306);
		}

	private:

		int ComunicateFunc(const int socket);

		int Identification(const int socket);

		//////////////////////////////////////////////////////////////////
		
		int SendHash(const int socket);

		int RentalConfirm(const int socket);

		//////////////////////////////////////////////////////////////////

		int ReturnUmbrellaCode(const int socket);

		int ReturnConfirm(const int socket);


		/////////////////////////////////////////////////////////////////
		//DB part
		int CheckRentalSpotID(int spotID);//part1
		int UmbrellaFindUsers(int umbrellaid);//part2
		bool UpdateUmbrellaLocation(int umbrellaID, int newSpotID, int newSlotID);//part3
};

#endif
