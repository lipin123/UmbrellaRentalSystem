#include "Network.h"
#include <cstring>
#include <cstdio>

#ifndef __JSON__
#define __JSON__
#include "json/json.h"
#endif



#define WAITING_LENGTH (100)
#define MAXIMUM_SOCKET (8)
#define MAXIMUM_CONNECTION (1000)

using namespace std;

char * strupr(char* string);

//listen 루프를 도는 함수
//port : 해당 루프가 데이터를 받을 port번호
//func : 패킷을 받았을때 처리할 함수 포인터(클라이언트 소켓, string 데이터)
int Network::ListenLoop(int port)
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
				ev.events = EPOLLIN|EPOLLERR;
				ev.data.fd = client;
				epoll_ctl(epfd,EPOLL_CTL_ADD, client, &ev);
			}
			//client socket으로 넘어온 데이터들
			else
			{
				//클라이언트 핸들
				client = evlist[i].data.fd;
				dataStr = buffer;

				switch(dataStreamRead(client, buffer))
				{
					case -1:
						cout<<"Data Length error!"<<endl;
						exit(1);
					case 1:
						cout<<"EOF detected!"<<endl;
						break;
				}

				//디버깅용
				if(DEBUG)
				{
					cout<<buffer<<endl;

					strupr(buffer);

					if((send(client, buffer, BUFFER_MAX_LEN, 0))<0)
					{
						perror("Error: Send failed\n");
						return -1;
					}

					cout<<epoll_ctl(epfd,EPOLL_CTL_DEL,client,&evlist[i])<<endl;;
					close(client);
				}
				///////////////////////
				else
					if(ComunicateFunc(client, dataStr))	//데이터를 클라이언트와 통신하는 함수로 넘겨줌
					{
						//1이 반환되면 통신이 이제 종료되므로 epoll에서 제거
						epoll_ctl(epfd,EPOLL_CTL_DEL,client,&evlist[i]);
					}
			}

		}
	}

	//종료시 서버 제거
	close(server);
	return 0;
}

int Network::dataStreamRead(int socket, char * buffer)
{
	unsigned int len = 0;			//현재까지 받은 데이터
	unsigned int remainLen;			//남은 받아야 하는 데이터
	unsigned int numByte = 0;		//방금 read로 받은 데이터
	char * buffer_ptr = buffer;

	//데이터를 받는 부분, remainLen까지 다 받기 전까지 계속해서 데이터를 받는다.
	memset(buffer,0x00,sizeof(buffer));	//buffer reset

	remainLen = sizeof(int);	//정수값을 받기 위한 함수
	while(remainLen != 0)
	{
		numByte = read(socket, buffer_ptr, remainLen);
		if(numByte == 0)
			return 1;
		buffer_ptr += numByte;
		remainLen -= numByte;
		len += numByte;
	}

	memcpy(&remainLen, buffer, 4);	//string 형태로 온 int 데이터를 강제로 집어넣음
	if((remainLen > BUFFER_MAX_LEN)||(remainLen<0))	//데이터 사이즈가 버퍼 크기를 초과하는지 체크
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

	return 0;
}

int Network::stringToJson(string data)
{
	Json::Reader reader;

	dataJson.clear();
	if(!reader.parse(data,dataJson))
		return 1;

	return 0;
}

char * strupr(char * string)
{
	int i = 0;
	while(1)
	{
		if(string[i] == NULL)
			return string;
		string[i] = toupper(string[i]);
		i++;
	}
}

