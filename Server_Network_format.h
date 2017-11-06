/////////////////////////////////////////////////////////
//
// 서버와의 네트워크 데이터 포멧을 규정하는 헤더입니다.
//
/////////////////////////////////////////////////////////

#ifndef __SERVER_NETWORK_FORMAT__
#define __SERVER_NETWORK_FORMAT__

#define U2S_UserInfo		0x00	//최초 연결시 유저의 정보를 보낼때 커맨드 바이트
#define U2S_ReqRentalPos	0x01	//대여소 위치 요청
#define S2U_RentalPos		0x02	//대여소 위치 응답
#define U2S_SelRentalSpot	0x03	//대여소 선택
#define S2U_RentalSpotInfo	0x04	//대여소 정보 응답
#define U2S_SelUmb			0x05	//우산 선택
#define S2U_UmbHash			0x06	//해당 우산 해시코드 응답
#define S2U_RentalComplete	0x07	//최종적으로 우산 대여가 완료됨



#endif
