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

///работает с сокетамми
/**
используется дл ясвязи с клиентом через winsoc2
*/

class c_server
{
	public:
		c_server();
		~c_server();
		///слушает заданный в config сокет, возвращает пришедшее сообщение
		/**
		\param [in] sleep_time - для настройки прослушки, сколько ждать перед следующей проверкой
		*/
		char* listen_network(int sleep_time);
		///Отправляет данные клиенту
		/**
		\param[in] data - отправляеые данные
		*/
		void send_to_client(const char* data);//TODO:sending
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
		printf("WSAStartup failed: %d\n", iResult);//TODO: parametr
	}else{
		printf("WSAStartup successful\n");//TODO: parametr
	}
	
	//create listener
	Listen = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	Connect = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(Listen == SOCKET_ERROR || Connect == SOCKET_ERROR){
		printf("Socket creating failed\n");//TODO: parametr
	}else{
		printf("Socket created\n");//TODO: parametr
	}
	
	Server.sin_family = AF_INET;
	Server.sin_port = htons(port);
	Server.sin_addr.s_addr = INADDR_ANY;
	
	if(bind(Listen,(SOCKADDR*)&Server,sizeof(Server))==SOCKET_ERROR){
		printf("can't bind socket\n");//TODO: parametr
	}else{
		printf("socket bind successful\n");//TODO: parametr
	}
	
	listen(Listen,SOMAXCONN);
}

c_server::~c_server(){
	delete msg;
	//ZeroMemory(msg,sizeof(msg));//TODO: solve this
	closesocket(Listen);
	closesocket(Connect);
	WSACleanup();
}

char* c_server::listen_network(int sleep_time){
	printf("listening...\n");//TODO: parametr
	while(true){
		cout<<".";
		if(Connect = accept(Listen, NULL,NULL)){
			cout<<endl;
			printf("client connected\n");//TODO: parametr
			recv(Connect, msg, MAX_PATH, 0);
			//send(Connect, "message from server\n", MAX_PATH,0);//TODO: oarametr
			cout<<msg<<endl;//TODO: selete this
			return msg;
			break;
		}
		Sleep(sleep_time);
	}
	return NULL;
}

void c_server::send_to_client(const char* data){
	cout<<"send func started\n";//TDOO: selete this
	send(Connect,data,MAX_PATH,0);
}
#endif
