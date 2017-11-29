#include "Network.h"
#include "Server_Network_format.h"
#include <cstring>
#include <cstdio>

#ifndef __JSON__
#define __JSON__
#include "json/json.h"
#endif

#define DEBUG 1

#define WAITING_LENGTH (100)
#define MAXIMUM_SOCKET (8)
#define MAXIMUM_CONNECTION (1000)

using namespace std;

char * strupr(char* string);

//listen 루프를 도는 함수
//port : 해당 루프가 데이터를 받을 port번호
int Network::ListenLoop(const int port)
{
	struct sockaddr_in server_addr, client_addr;	//주소값 변수들
	int server, client;								//소켓 변수

	struct epoll_event ev;							//임시로 쓰는 epoll_event 변수
	struct epoll_event evlist[MAXIMUM_SOCKET];		//트리거가 발생한 이벤트를 담는 배열
	int epfd;										//epoll의 파일 디스크립터
	int numOfEvent;									//받은 event의 숫자

	//데이터를 받기위한 변수들
	char buffer[BUFFER_MAX_LEN];		//데이터를 받을 버퍼
	string dataStr;


	//port number check
	if((port < 1024)||(port>0xFFFF))
	{
		cout<<"Server port should be less than 1024"<<endl;
		return -1;
	}


	//server socket create
	if((server = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		cout<<"Server : Can't open stream socket"<<endl;
		return -1;
	}

	//address space clear
	memset(&server_addr, 0x00, sizeof(server_addr));	//memory reset

	//server_addr setting
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);


	//server socket binding
	if(bind(server, (struct sockaddr *)&server_addr, sizeof(server_addr))< 0)
	{
		cout<<"Server can't bind local address"<<endl;
		return -1;
	}

	//server listening
	if(listen(server, WAITING_LENGTH)<0)
	{
		cout<<"Server can't listen"<<endl;
		return -1;
	}


	//epoll create
	if((epfd = epoll_create(MAXIMUM_CONNECTION)) < 0)
	{
		cout<<"Epoll Create Error"<<endl;
		return -1;
	}


	//listening socket setting
	ev.events = EPOLLIN | EPOLLERR;	//들어왔을때랑 에러났을때 인터럽트
	ev.data.fd = server;
	epoll_ctl(epfd,EPOLL_CTL_ADD, server, &ev);	//listening 소켓 입력

	//epoll listening loop
	//exitFlag가 true가 되면 루프 종료
	while(!exitFlag)
	{
		numOfEvent = epoll_wait(epfd, evlist, MAXIMUM_SOCKET,-1);	//데이터가 들어오면 인터럽트

		//디버깅용
		if(DEBUG)
			cout<<"one round"<<endl;
		//////////////

		for(int i = 0; i<numOfEvent; i++)
		{
			socklen_t clilen = sizeof(clilen);
			//listening socket이 받았을 때
			if(evlist[i].data.fd == server)
			{
				client = accept(server, (struct sockaddr *)&client_addr, &clilen);


				//epoll 넣기
				ev.events = EPOLLIN|EPOLLERR;
				ev.data.fd = client;
				epoll_ctl(epfd,EPOLL_CTL_ADD, client, &ev);
			}
			//client socket으로 넘어온 데이터들
			else
			{
				//클라이언트 핸들
				client = evlist[i].data.fd;

				switch(dataStreamRead(client))
				{
					case -1:
						if(DEBUG)
							cout<<"Data Length error!"<<endl;
						exit(1);
					case 0:
						if(ComunicateFunc(client))	//데이터를 클라이언트와 통신하는 함수로 넘겨줌
						{
							sendFIN(client);
							//1이 반환되면 통신이 이제 종료되므로 epoll에서 제거
							close(client);
							epoll_ctl(epfd,EPOLL_CTL_DEL,client,&evlist[i]);
						}
						break;
					case 1:
						if(DEBUG)
							cout<<"EOF detected!"<<endl;
						break;
					case 2:	//Identification error
						if(DEBUG)
							cout<<"Identification fail"<<endl;
						close(client);
						epoll_ctl(epfd,EPOLL_CTL_DEL,client,&evlist[i]);
						break;
				}
				
			}
		}
	}

	//종료시 서버 제거
	close(server);
	return 0;
}

int Network::dataStreamRead(const int socket)
{
	unsigned int len = 0;			//현재까지 받은 데이터
	unsigned int remainLen;			//남은 받아야 하는 데이터
	unsigned int numByte = 0;		//방금 read로 받은 데이터
	char buffer[BUFFER_MAX_LEN];
	char * buffer_ptr = buffer;

	//데이터를 받는 부분, remainLen까지 다 받기 전까지 계속해서 데이터를 받는다.
	memset(buffer,0x00,sizeof(buffer));	//buffer reset

	remainLen = 8;	//정수값을 받기 위한 함수
	while(remainLen != 0)
	{
		numByte = read(socket, buffer_ptr, remainLen);
		buffer_ptr += numByte;
		remainLen -= numByte;
		len += numByte;
		if(numByte == 0)
			return 1;
		if(DEBUG)
			cout <<"remainLen : "<< remainLen <<endl;
	}

	string tem = buffer;
	remainLen = atoi(tem.c_str());

	if(DEBUG)
		cout<<"Byte Received : "<<remainLen<<endl;
	if((remainLen > BUFFER_MAX_LEN)||(remainLen<=0))	//데이터 사이즈가 버퍼 크기를 초과하는지 체크
		return -1;


	buffer_ptr = buffer;
	len = 0;
	numByte = 0;

	while(remainLen != 0)
	{
		numByte = read(socket, buffer_ptr, remainLen);
		buffer_ptr += numByte;
		remainLen -= numByte;
		len += numByte;
	}

	string data = buffer;

	if(DEBUG)
		cout<<"Data Received"<<endl<<data<<endl;

	if(stringToJson(data))
		return -1;

	//매 패킷마다 Identification 확인
	if(Identification(socket))
		return 2;

	return 0;
}

int Network::dataStreamWrite(const int socket, Json::Value &data)
{
	Json::StyledWriter writer;
	string sendDataStr = writer.write(data);
	int strLen = sendDataStr.length();

	
	char sendSize[8];
	int strLen_tem = strLen;
	memset(sendSize, '0', 8);
	for(int i = 0;i < 8; i++)
	{
		sendSize[7-i] += strLen_tem % 10;
		strLen_tem /= 10;
	}
	cout << sendSize<<endl;
	write(socket, sendSize, 8);
	write(socket, sendDataStr.c_str(), strLen);
	return 0;
}

int Network::stringToJson(const string &data)
{
	Json::Reader reader;

	dataJson.clear();
	if(!reader.parse(data,dataJson))
		return 1;

	return 0;
}

int Network::sendFIN(const int socket)
{
	Json::Value sendData;
	
	sendData["command"] = FIN;
	
	dataStreamWrite(socket, sendData);

	return 1;
}
