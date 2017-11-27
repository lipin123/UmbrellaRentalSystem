/////////////////////////////////////////////////////////
//
// 서버와의 네트워크 JSON 데이터 포멧을 규정하는 헤더입니다.
//
/////////////////////////////////////////////////////////

#ifndef __SERVER_NETWORK_FORMAT__
#define __SERVER_NETWORK_FORMAT__

#include "json/json.h"

//기본 커멘드 바이트
#define ACK					(0xFF)	//데이터를 받았다는 의미로 보내는 ACK
#define U2S_UserIden		(0x00)	//최초 연결시 유저 Identification
#define R2S_RentalIden		(0x10)	//최초 연결시 대여소 identification

//int 정수형
//double 소수 형태
//string 문자열 형태

/*
 * 최초 유저 연결
 * int command
 * int userID
 *
 */


/*
 * 대여소 최초 연결
 * int command
 * int spotID
 */


/*
 * 모든 최초 연결시 올바른 유저 혹은 대여소일 경우 ACK
 * int command(ACK)
 * int ID
 *
 */

///////////////////////////////////////////////////////////////////////////
// 대여 시나리오
// ////////////////////////////////////////////////////////////////////////

//기본 대여소 커맨드 바이트
#define U2S_ReqRentalPos	(0x01)	//대여소 위치 요청
#define S2U_RentalPos		(0x02)	//대여소 위치 응답
#define U2S_SelSpot			(0x03)	//대여소 선택
#define S2U_SpotInfo		(0x04)	//대여소 정보 응답
#define U2S_SelUmb			(0x05)	//우산 선택
#define S2U_UmbHash			(0x06)	//해당 우산 해시코드 응답
#define S2U_RentalComplete	(0x07)	//최종적으로 우산 대여가 완료됨

#define R2S_SendHash		(0x11)	//QR코드로 읽은 해시코드 전송
#define S2R_VerifyCom		(0x12)	//해시코드에 해당하는 명령 전송
#define R2S_RentConfirm		(0x13)	//우산 대여 후 이상유무 및 현 상태 전송

struct SpotLocation
{
	int eastLongitude;
	int northLatitude;
	int spotID;
	int numOfUmb;
};


/*
 * 대여소 좌표 요청
 * int command(U2S_ReqRentalPos)
 * int userID
 * double E
 * double N
 * double width
 * double height
 *
 */

/*
 * 대여소 좌표 제공
 * int command(S2U_RentalPos)
 * int userID
 * int numOfSpot
 * SpotLocation
 * [
 * 		double E
 *      double N
 *      int spotID
 *      int numOfUmb
 *      ,
 *      ...
 * ]
 * 
 */

/*
 * 대여소 선택
 * int command(U2S_SelSpot)
 * int userID
 * int spotID
 */

/*
 * 대여소 정보 제공
 * int command(S2U_SpotInfo)
 * int userID
 * int spotID
 * int x
 * int y
 * string umbStorage
 */



/*
 * 우산 선택
 * int command(U2S_SelUmb)
 * int userID
 * int spotID
 * int umbNum
 */

/*
 * 해당 선택 해싱코드 제공
 * int command(S2U_UmbHash)
 * int userID
 * int spotID
 * string hashCode(32바이트)
 */

/*
 * 대여소가 입력받은 해싱코드 전달
 * int command(R2S_SendHash)
 * int spotID
 * int userID
 * string hashCode(32바이트)
 */

/*
 * 해싱코드 확인 후 대여해줘야할 우산 전달(만약 아닐시 우산 번호에 -1)
 * int command(S2R_VerifyCom)
 * int spotID
 * int umbID;
 */

/*
 * 대여 확인 후 현 대여소 상태 업데이트
 * int command(R2S_RentConfirm)
 * int spotID
 * int status
 * string umbStorage
 */

/*
 * 대여 완료후 유저에게 이상 유무 및 우산 정보 전달
 * int command(S2U_RentalComplete)
 * int umbID
 * int status
 */

/////////////////////////////////////////////////////////////////////////////
// 반납 시나리오
// //////////////////////////////////////////////////////////////////////////

#define R2S_ReturnUmbrellaCode		(0x21)
#define S2R_VerifyUmbrellaCode		(0x22)
#define R2S_ReturnConfirm			(0x23)

/*
 * 우산 반납 코드 서버에 전송
 * int command(R2S_ReturnUmbrellaCode)
 * int spotID
 * int umbID
 */

/*
 * 우산 코드 인증 및 확인
 * int command(S2R_VerifyUmbrellaCode)
 * int spotID
 * int userID
 * int umbID
 */

/*
 * 반납 최종 확인 및 최신화
 * int command(R2S_ReturnConfirm)
 * int spotID
 * int umbID
 * int status
 * int returnPlace
 * string umbStorage
 */

#endif
