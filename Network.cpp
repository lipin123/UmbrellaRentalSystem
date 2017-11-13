#include "Network.h"
#include <cstring>
#include <cstdio>

#define WAITING_LENGTH (100)
#define MAXIMUM_SOCKET (8)
#define MAXIMUM_CONNECTION (1000)

using namespace std;

char * strupr(char* string);

int ListenLoop(int port)
{
	struct sockaddr_in server_addr, client_addr;
	struct epoll_event ev;
	struct epoll_event evlist[MAXIMUM_SOCKET];
	int server, client;
	int epfd;
	int n;

	if(port < 1024)
	{
		cout<<"Server port should be less than 1024"<<endl;
		return -1;
	}

	

	if((server = socket(AF_INET, SOCK_STREAM, 0)) == -1)	//socket create
	{
		cout<<"Server : Can't open stream socket"<<endl;
		return -1;
	}

	memset(&server_addr, 0x00, sizeof(server_addr));	//memory reset

	//server_addr setting
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);


	if(bind(server, (struct sockaddr *)&server_addr, sizeof(server_addr))< 0)
	{
		cout<<"Server can't bind local address"<<endl;
		return -1;
	}

	if(listen(server, WAITING_LENGTH)<0)
	{
		cout<<"Server can't listen"<<endl;
		return -1;
	}

	if((epfd = epoll_create(MAXIMUM_CONNECTION)) < 0)
	{
		cout<<"Epoll Create Error"<<endl;
		return -1;
	}
	
	ev.events = EPOLLIN;
	ev.data.fd = server;
	epoll_ctl(epfd,EPOLL_CTL_ADD, server, &ev);

	while(!exitFlag)
	{
		n = epoll_wait(epfd, evlist, MAXIMUM_SOCKET,100);
		for(int i = 0; i<n; i++)
		{
			socklen_t clilen = sizeof(clilen);
			if(evlist[i].data.fd == server)
			{
				client = accept(server, (struct sockaddr *)&client_addr, &clilen);
				ev.events == EPOLLIN;
				ev.data.fd = client;
				epoll_ctl(epfd,EPOLL_CTL_ADD, client, &ev);
			}else
			{
				char buffer[256];
				char * buffer_ptr = buffer;
				int len = 0;
				int maxLen = sizeof(buffer);
				int numByte = 0;

				//클라이언트 핸들
				client = evlist[i].data.fd;
				while(maxLen != 0)
				{
					numByte = recv(client, buffer_ptr, maxLen, 0);
					buffer_ptr += numByte;
					maxLen -= numByte;
					len += numByte;
				}
				maxLen = sizeof(buffer);
				buffer_ptr = buffer;
				len = 0;

				cout<<buffer<<endl;

				strupr(buffer);

				if((send(client, buffer, maxLen, 0))<0)
				{
					perror("Error: Send failed\n");
					exit(1);
				}

				cout<<epoll_ctl(epfd,EPOLL_CTL_DEL,client,&evlist[i])<<endl;;
				close(client);
			}

		}
	}
	close(server);
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

int main(void)
{
	ListenLoop(5000);
	return 0;
}
