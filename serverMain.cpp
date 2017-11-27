#include <iostream>
#include "UserNetwork.h"
#include "SpotNetwork.h"
#include <pthread.h>
#include <unistd.h>

#define USER_PORT (5000)

using namespace std;

void * UserNetworkFunc(void * d)
{
	UserNetwork u;

	u.ListenLoop(USER_PORT);
}

int main(int argc, char** argv)
{
	pthread_t userNet;
	pthread_t spot;

	//cout<<pthread_create(&userNet, NULL, UserNetworkFunc, NULL)<<endl;

	SpotNetwork s;
	s.ListenLoop(USER_PORT);
	//pthread_join(userNet,NULL);
	return 0;
}
