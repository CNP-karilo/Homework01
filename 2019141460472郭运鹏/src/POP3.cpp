#include"POP3.h"
#include"Socket.h"
#include"Base64Test.h"
#include <windows.h>
#include<fstream>
#include<string>

using namespace std;

void POP3_163(string userName, string passWord) {
	string thePath = "2.txt";//�ʼ��ı���·��
	string whichEmail = "1";//ѡ���ķ��ʼ�
	ofstream outfile(thePath, ios::out);//�ļ���
	string theContent = "";//�ʼ�������
	string temp = "";//��ʱƴ�����ַ���

	LPTSTR lpPath = new char[MAX_PATH];
	LPTSTR POP = new char[20];
	strcpy(lpPath, "C:\\ip.ini");
	GetPrivateProfileString("POP", "163", "", POP, 20, lpPath);

	Socket sock(POP, 110);
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	//�����û���
	temp = "user " + userName + "@163.com\r\n";
	cout << "c:" << temp << endl;
	sock.sendSocketMessage(temp.data());//
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	//����
	temp = "pass " + passWord + "\r\n";
	cout << "c:" << "pass ***********" << endl;
	sock.sendSocketMessage(temp.data());//
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	cout << "c:" << "stat" << endl;
	sock.sendSocketMessage("stat\r\n");
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	cout << "c:" << "list" << endl;
	sock.sendSocketMessage("list\r\n");
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	cout << "��ѡ����Ҫ��ȡ��һ���ʼ�,�����������֣�" << endl;
	cin >> whichEmail;

	temp = "retr " + whichEmail + "\r\n";
	cout << "c:" << "retr " << whichEmail << endl;
	sock.sendSocketMessage(temp.data());



	while (1)//���õ�������ѭ������received���������ù���ʱ�ж�
	{

		sock.reviceSocketMessage();
		//	cout << "Server : " << sock.getReviceMessage() << endl;

		string test = sock.getReviceMessage();
		theContent += test;

		if (test.length() == 0)
		{
			break;
		}
	}

	outfile << theContent;

	temp = "�ʼ��ɹ�д�뵽" + thePath;
	cout << temp << endl;

	cout << "c:" << "quit" << endl;
	sock.sendSocketMessage("quit\r\n");
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;
}

bool testPopAuthor_163(string userName, string passWord) {

	string temp = "";

	Socket sock("121.195.178.52", 110); //����ip��һ�£��˿ڶ���25    59.111.192.150ѧУ��
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	temp = "user " + userName + "@163.com\r\n";
	cout << "c:" << temp << endl;
	sock.sendSocketMessage(temp.data());//"user lmom_xa@163.com\r\n"
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	//����
	temp = "pass " + passWord + "\r\n";
	cout << "c:" << "pass ***********" << endl;
	sock.sendSocketMessage(temp.data());
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	//����һ��string ʹ��find�ж������Ƿ�ɹ�
	string test(sock.getReviceMessage());//
	int exist = test.find("+OK");
	if (exist == string::npos) {

		return false;
	}

	else {
		return true;
	}
}

void POP3_qq(std::string userName, std::string passWord) {
	string thePath = "1.txt";//�ʼ��ı���·��
	string whichEmail = "1";//ѡ���ķ��ʼ�
	ofstream outfile(thePath, ios::out);//�ļ���
	string theContent = "";//�ʼ�������
	string temp = "";//��ʱƴ�����ַ���

	LPTSTR lpPath = new char[MAX_PATH];
	LPTSTR QQ = new char[20];
	strcpy(lpPath, "C:\\ip.ini");
	GetPrivateProfileString("POP", "qq", "", QQ, 20, lpPath);

	Socket sock(QQ, 110);
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	//�����û���
	temp = "user " + userName + "@qq.com\r\n";
	cout << "c:" << temp << endl;
	sock.sendSocketMessage(temp.data());
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	//����
	temp = "pass " + passWord + "\r\n";
	cout << "c:" << "pass ***********" << endl;
	sock.sendSocketMessage(temp.data());
	cout << sock.getReviceMessage() << endl;

	cout << "c:" << "stat" << endl;
	sock.sendSocketMessage("stat\r\n");
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	cout << "c:" << "list" << endl;
	sock.sendSocketMessage("list\r\n");

	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	sock.sendSocketMessage(" \r\n");
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;


	cout << "��ѡ����Ҫ��ȡ��һ���ʼ�,�����������֣�" << endl;
	cin >> whichEmail;

	temp = "retr " + whichEmail + "\r\n";
	cout << "c:" << "retr " << whichEmail << endl;
	sock.sendSocketMessage(temp.data());



	while (1)
	{

		sock.reviceSocketMessage();
		cout << "Server" << sock.getReviceMessage() << endl;
		//	cout << "Server : " << sock.getReviceMessage() << endl;

		string test = sock.getReviceMessage();
		theContent += test;

		if (test.length() == 0)
		{
			break;
		}
	}

	outfile << theContent;

	temp = "�ʼ��ɹ�д�뵽" + thePath;
	cout << temp << endl;

	cout << "c:" << "quit" << endl;
	sock.sendSocketMessage("quit\r\n");
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;
}

bool testPopAuthor_qq(std::string userName, std::string passWord) {

	string temp = "";

	Socket sock("121.51.131.77", 110);
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	temp = "user " + userName + "@qq.com\r\n";
	cout << "c:" << temp << endl;
	sock.sendSocketMessage(temp.data());
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	//����
	temp = "pass " + passWord + "\r\n";
	cout << "c:" << "pass ***********" << endl;
	sock.sendSocketMessage(temp.data());
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	//����һ��string ʹ��find�ж������Ƿ�ɹ�
	string test(sock.getReviceMessage());//
	int exist = test.find("+OK");
	if (exist == string::npos) {

		return false;
	}

	else {
		return true;
	}
}