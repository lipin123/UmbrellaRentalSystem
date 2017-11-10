#include "Network.h"

#define WAITING_LENGTH (100)
#define MAXIMUM_SOCKET (8)
#define MAXIMUM_CONNECTION (1000)

using namespace std;

int ListenLoop(int port)
{
	struct sockaddr_in server_addr, client_addr;
	struct epoll_event ev;
	struct epoll_event evlist[MAXIMUM_SOCKET];
	int server, client;
	int efd;
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

	memset(server_addr, 0x00, sizeof(server_addr));	//memory reset

	//server_addr setting
	server_addr.sin_family = AF_INET;
	server_addr.sin_family.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = port;


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
	epoll_ctl(efd,EPOLL_CTL_ADD, server, &ev);

	while(!exitFlag)
	{
		n = epoll_wait(efd, evlist, MAXIMUM_SOCKET,100);
		
		for(int i = 0; i<n; i++)
		{
			if(events[i].data.fd == server)
			{
				client = accept(server, (struct sockaddr *)&clientaddr, sizeof(client_addr));
				ev.events == EPOLLIN;
				ev.data.fd = client;
				epoll_ctl(efd,EPOLL_CTL_ADD, client, &ev);
			}else
			{
				//클라이언트 핸들
			}

		}
	}
	close(server);
	return 0;
}
