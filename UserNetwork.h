#ifndef __USER_NETWORK__
#define __USER_NETWORK__

#include "Server_Network_format.h"

class UserNetwork
{
	private:
		int listenSocket;
		struct sockaddr_in server_addr;

	public:
		int UserNetworkLoop();

	public:
		UserNetwork();

};

#endif
