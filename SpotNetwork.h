#ifndef __SPOT_NETWORK__
#define __SPOT_NETWORK__

#include "Server_Network_format.h"
#include "Network.h"
#include <iostream>

class SpotNetwork : public Network
{
	private:
		int ComunicateFunc(const int socket);

		int Identification(const int socket);
};
