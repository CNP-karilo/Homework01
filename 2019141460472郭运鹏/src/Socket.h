#pragma once
#include "winsock2.h"  
#include <iostream>  
#pragma comment(lib, "ws2_32.lib")
#include<memory>

class Socket
{
public:
	Socket();
	Socket(const char* ip, const int port);
	bool initialize(const char* ip, const int port);
	bool sendSocketMessage(const char* mesg);
	bool reviceSocketMessage();
	void CloseSocket();
	char* getReviceMessage();
private:

	WSADATA wsd; //WSADATA����  
	SOCKET sHost; //�ͻ����׽���  
	SOCKADDR_IN addr; //��������ַ     sockaddr��Ŀ�ĵ�ַ�Ͷ˿ڻ�ϣ�sockaddr_in����    

	char bufRecv[512];//�������ݻ�����  
	int retVal; //����ֵ  
};

