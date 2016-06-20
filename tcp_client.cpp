#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>

using namespace std;

void usage(string _port)
{
	cout<<_port<<"[remote ip][remote port]"<<endl;
}

int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		usage(argv[0]);
		exit(1);
	}

	string ip=argv[1];
	int r_port=atoi(argv[2]);

	int sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock<-1)
	{
		cout<<strerror(errno)<<endl;
		exit(1);
	}

	struct sockaddr_in remote;
	remote.sin_family=AF_INET;
	remote.sin_port=htons(r_port);
	remote.sin_addr.s_addr=inet_addr(ip.c_str());
	int ret=connect(sock,(struct sockaddr*)&remote,sizeof(remote));
	if(ret<0)
	{
		cout<<strerror(errno)<<endl;
	}
	
	string msg;
	while(1)
	{
		cout<<"plenter enter: ";
		cin >> msg;
		write(sock,msg.c_str(),msg.size());
	}
	return 0;
}
