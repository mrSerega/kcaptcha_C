#ifndef SERVER_H
#define SERVER_H

#define WS_VERSION MAKEWORD(2,1)

#include <winsock2.h>
#include <windows.h>
#include <iostream>

using namespace std;

#include "../parsers/iniparser.h"
#include "../defines.h"
#include "../casts/strtoint.h"

class c_server
{
	public:
		c_server();
		~c_server();
		char* listen_network();
	protected:
		const char* ip;
		int port;
		char* msg;
		
		WSADATA wsaData;
		SOCKET Listen;
		SOCKET Connect;
		SOCKADDR_IN Server;	
};

c_server::c_server(){
	IniParser parser(CONFIG_FILE_NAME);
	ip = parser.get_value(NETWORK_SECTION_NAME,IP_VARIABLE_NAME);
	port = strToInt(parser.get_value(NETWORK_SECTION_NAME,PORT_VARIABLE_NAME));
	
	msg = new char[MAX_PATH];
	
	int iResult;
	//init winsok
	iResult = WSAStartup(WS_VERSION,&wsaData);
	if(iResult){
		printf("WSAStartup failed: %d\n", iResult);
	}else{
		printf("WSAStartup successful\n");
	}
	
	//create listener
	Listen = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	Connect = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(Listen == SOCKET_ERROR || Connect == SOCKET_ERROR){
		printf("Socket creating failed\n");
	}else{
		printf("Socket created\n");
	}
	
	Server.sin_family = AF_INET;
	Server.sin_port = htons(port);
	Server.sin_addr.s_addr = INADDR_ANY;
	
	if(bind(Listen,(SOCKADDR*)&Server,sizeof(Server))==SOCKET_ERROR){
		printf("can't bind socket\n");
	}else{
		printf("socket bind successful\n");
	}
	
	listen(Listen,SOMAXCONN);
}

c_server::~c_server(){
	delete msg;
	ZeroMemory(msg,sizeof(msg));
	closesocket(Listen);
	closesocket(Connect);
	WSACleanup();
}

char* c_server::listen_network(){
	printf("listening...\n");
	while(true){
		if(Connect = accept(Listen, NULL,NULL)){
			printf("client connected\n");
			recv(Connect, msg, sizeof(msg), 0);
			send(Connect, "message from server\n", MAX_PATH,0);
			//return msg;
			break;
		}
		Sleep(100);
	}
	
	printf("message from client: %s", msg);
	return NULL;
}

#endif
