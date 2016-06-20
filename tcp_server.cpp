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

const int g_backlog=5;

void usage(string _port)
{
	cout<<"usage: "<<_port<<"[ip][port]"<<endl;
}

static int start(const string &ip,const int &_port)
{
	int listen_sock=socket(AF_INET,SOCK_STREAM,0);//创建套接字，参数：ipv4协议，字节流套字
	if(listen_sock<0)
	{
		cerr<<strerror(errno)<<endl;
		exit(1);
	}
	
	int opt=1;
	if(setsockopt(listen_sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt))<0)
	{
		cerr<<strerror(errno)<<endl;
		exit(0);
	}

	struct sockaddr_in local;  //填充sock信息
	local.sin_family=AF_INET;
	local.sin_port=htons(_port);
	local.sin_addr.s_addr=inet_addr(ip.c_str());
	if(bind(listen_sock,(struct sockaddr*)&local,sizeof(local))<0)  //把sock和填充的信息进行绑定
	{
		cerr<<strerror(errno)<<endl;
		exit(2);
	}
	
	if(listen(listen_sock,g_backlog)<0)  //把该套接字设置为监听套接字
	{
		cerr<<strerror(errno)<<endl;
		exit(3);
	}
	return listen_sock;
}

void *thread_run(void *arg)
{
	int sock =(int)arg;
	char buf[1024];
	while(1)
	{
		memset(buf,'\0',sizeof(buf));
		ssize_t _size=read(sock,buf,sizeof(buf)-1);
		if(_size>0)
		{
			buf[_size]='\0';
		}
		else if(_size==0)
		{
			cout<<"client close..."<<endl;
			break;
		}
		else
		{
			cout<<strerror(errno)<<endl;
		}
		cout<<"client# "<<buf<<endl;
	}
	close(sock);
	return NULL;
}

int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		usage(argv[0]);
		exit(1);
	}
	struct sockaddr_in client;
	socklen_t len=sizeof(client);
	string ip=argv[1];
	int port=atoi(argv[2]);
	int listen_sock=start(ip,port);
	while(1)
	{
		int new_fd=accept(listen_sock,(struct sockaddr*)&client,&len);
		if(new_fd<0)
		{
			cerr<<strerror(errno)<<endl;
			continue;
		}
		cout<<"get a connect..."<<" sock :"<<new_fd<<" ip:"<<inet_ntoa(client.sin_addr)<<" port:"<<ntohs(client.sin_port)<<endl;
#ifdef _V1_
		char buf[1024];
		while(1)
		{
			string _client=inet_ntoa(client.sin_addr);
			ssize_t _size=read(new_fd,buf,sizeof(buf)-1);
			if(_size>0)
			{
				buf[_size]='\0';
			}
			else if(_size==0)
			{
				//client close
				cout<<_client<<"close..."<<endl;
				break;
			}
			else
			{
				cout<<strerror(errno)<<endl;
			}
			cout<<"client#:"<<buf<<endl;
		}
#elif _V2_
		cout <<"v2"<<endl;
		pid_t id=fork();
		if(id==0)
		{
			string _client=inet_ntoa(client.sin_addr);
			int _port=ntohs(client.sin_port);
			close(listen_sock);
			char buf[1024];
			while(1)
			{
				memset(buf,'\0',sizeof(buf));i
				ssize_t _size=read(new_fd,buf,sizeof(buf)-1);
				if(_size>0)
				{
					buf[_size]='\0';
				}
				else if(_size==0)
				{
					cout<<_client<<"cloise..."<<endl;
					break;
				}
				else
				{
					cout<<strerror(errno)<<endl;
				}
				cout<<_client<<" "<<_port<<"# "<<buf<<endl;
			}
			close(new_fd);
			exit(0);
		}
		else if(id>0)
		{
			close(new_fd);
		}
		else
		{}
#elif _V3_
		pthread_t tid;
		pthread_create(&tid,NULL,thread_run,(void*)new_fd);
		pthread_detach(tid);
#else
		cout<<"default"<<endl;
#endif
	}
	return 0;
}
