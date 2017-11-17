#include "SpotNetwork.h"


using namespace std;

int SpotNetwork::ComunicateFunc(const int socket)
{
	switch(dataJson["command"].asInt())
	{
		case R2S_SendHash:	//

		case R2S_RentConfirm:
	}
}

int SpotNetwork::Identification(const int socket)
{
	int spotID = dataJson["ID"].asInt();

	if(dataJson["command"].asInt() == R2S_RentalIden)
	{
		Json::Value sendData;

		/////////////////////////////////////////////////////
		//	DB part
		//	return 1 if it's not right RentalSpot ID
		////////////////////////////////////////////////////
		
		sendData["spotID"] = spotID;
		sendData["command"] = ACK;

		dataStreamWrite(socket, sendData);

		return 0;
	}else
		return 1;
}
