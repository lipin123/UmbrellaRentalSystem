#ifndef __SPOT_NETWORK__
#define __SPOT_NETWORK__

#include "Server_Network_format.h"
#include "Network.h"
#include <iostream>
#include "MyDB.h";
class SpotNetwork : public Network
{
private:

	int ComunicateFunc(const int socket);

	int Identification(const int socket);

	int ReturnUmbrellaCode(const int socket);

	int ReturnConfirm(const int socket);

	int CheckRentalSpotID(int spotID);//part1
	int UmbrellaFindUsers(int umbrellaid);//part2
	bool UpdateUmbrellaLocation(int umbrellaID, int newSpotID, int newSlotID);//part3
};
