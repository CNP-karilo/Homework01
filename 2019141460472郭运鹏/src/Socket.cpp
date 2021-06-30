#include"Socket.h"

#include<string>
#include<iostream>

using namespace std;

Socket::Socket() {
	initialize("127.0.0.1", 4999);
};
Socket::Socket(const char* ip, const int port) {
	initialize(ip, port);
};
void Socket::CloseSocket() {
	closesocket(sHost);
	WSACleanup();
}
bool Socket::initialize(const char* ip, const int port) {
	WSADATA wsd;//��ʼ�����绷��
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)//�����׽��ֿ�  ʹ��2.2�汾��Socket����Ӧ�Ŀ��
	{
		cout << "WSAStartup failed!" << endl;//����ʧ�ܵ���ʾ
		return -1;
	}

	//�����׽���  
	sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//socket(int af, int type,int protocol) AF_INET��ʾIPV4��ַ��AP_INET6��ʾIPV6   SOCK_STREAM������ʽ/�������ӣ�TCP)
	if (INVALID_SOCKET == sHost)//�������ʧ�ܵķ���
	{
		cout << "socket failed!" << endl;
		WSACleanup();//�ͷ��׽�����Դ�������Socket��İ�  
		return  -1;
	}


	addr.sin_family = AF_INET;
	//�������ͨ���� ���� c++ ����  sdl �ĳɷ�
	addr.sin_addr.s_addr = inet_addr(ip);//���÷���˵�ַ����һ��IP�ַ���ת��Ϊһ�������ֽ��������ֵ
	addr.sin_port = htons((short)port);//htons���˿ں��������ֽ���ת��Ϊ�����ֽ��������ֵ
	int nServAddlen = sizeof(addr);


	//���ӷ�����  
	retVal = connect(sHost, (LPSOCKADDR)&addr, sizeof(addr));
	if (SOCKET_ERROR == retVal)
	{
		cout << "connect failed!" << endl;
		closesocket(sHost); //�ر��׽���  
		WSACleanup(); //�ͷ��׽�����Դ  
		return -1;
	}
}


bool Socket::sendSocketMessage(const char* mesg) {

	retVal = send(sHost, mesg, strlen(mesg), 0); //    �ú����ĵ�һ������ָ�����Ͷ��׽������������ڶ�������ָ��һ�����Ӧ�ó���Ҫ�������ݵĻ�����������������ָ��ʵ��Ҫ���͵����ݵ��ֽ��������ĸ�����һ����0��
	if (SOCKET_ERROR == retVal)
	{
		cout << "send failed!" << endl;
		closesocket(sHost); //�ر��׽���  
		WSACleanup(); //�ͷ��׽�����Դ  
		return -1;
	}
}

bool Socket::reviceSocketMessage() {

	ZeroMemory(bufRecv, 512);

	//���ý��ճ�ʱ
	int timeout = 4000;
	setsockopt(sHost, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));
	setsockopt(sHost, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

	int theRecLen = recv(sHost, bufRecv, 512, 0);
	if (SOCKET_ERROR == retVal)
	{
		cout << "revice failed!" << endl;
		closesocket(sHost); //�ر��׽���  
		WSACleanup(); //�ͷ��׽�����Դ  
		return false;
	}
	return true;

}

char* Socket::getReviceMessage()
{
	return bufRecv;
}