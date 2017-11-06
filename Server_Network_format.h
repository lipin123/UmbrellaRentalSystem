/////////////////////////////////////////////////////////
//
// 서버와의 네트워크 데이터 포멧을 규정하는 헤더입니다.
//
/////////////////////////////////////////////////////////

#ifndef __SERVER_NETWORK_FORMAT__
#define __SERVER_NETWORK_FORMAT__

//다음은 각 네트워크에서 커맨드 바이트들
#define ACK					0xFF	//데이터를 받았다는 의미로 보내는 ACK

#define U2S_UserIden		0x00	//최초 연결시 유저 identification
#define U2S_ReqRentalPos	0x01	//대여소 위치 요청
#define S2U_RentalPos		0x02	//대여소 위치 응답
#define U2S_SelRentalSpot	0x03	//대여소 선택
#define S2U_RentalSpotInfo	0x04	//대여소 정보 응답
#define U2S_SelUmb			0x05	//우산 선택
#define S2U_UmbHash			0x06	//해당 우산 해시코드 응답
#define S2U_RentalComplete	0x07	//최종적으로 우산 대여가 완료됨

#define R2S_RentalIden		0x10	//최초 연결시 대여소 identification
#define R2S_SendHash		0x11	//QR코드로 읽은 해시코드 전송
#define S2R_VerifyCom		0x12	//해시코드에 해당하는 명령 전송
#define R2S_RentConfirm		0x13	//우산 대여 후 이상유무 및 현 상태 전송

struct RentalSpotBrief
{
	int eastLongitude;
	int northLatitude;
	int rentalSpotID;
	int numOfUmb;
};

class UserIdentification
{
	public:
		int userID;
};

class RentalSpotRequest
{
	public:
		int eastLongitude;
		int northLatitude;
		double width;
		double height;
};

class RentalSpotPosition
{
	public:
		int numOfSpot;
		RentalSpotBrief * data;
};

class SelectRentalSpot
{
	public:
		int rentalSpotID;
};

class RentalSpotInfo
{
	public:
		int rentalSpotID;
		int x, y;
		char * umbStorage;
};

class SelectUmbrella
{
	public:
		int umbNum;
};

class UmbrellaHash
{
	public:
		char hashCode[32];
};

class RentalSpotIdentification
{
	public:
		int rentalSpotID;
};

class SendHash
{
	public:
		char hashCode[32];
};

class VerifyCom
{
	public:
		int status;
		int umbID;
};

class RentConfirm
{
	public:
		int status;
		char * umbStorage;
};

#endif
