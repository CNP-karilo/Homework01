#include"SMTP.h"
#include"Socket.h"
#include"Base64Test.h"
#include <windows.h>
#include<Winbase.h>
#include<string>
using namespace std;



void SMTP_163(string userName, string passWord) {//n���ж���qq���仹��163����

	string toWho = "";//�ռ�������
	string theSubject = "";//���͵�����
	string theContent = "";//Ҫ���͵�����
	string temp = "";//��������ƴ�ӱ�׼Э����ַ���

	LPTSTR lpPath = new char[MAX_PATH];
	LPTSTR S163 = new char[20];
	strcpy(lpPath, "C:\\ip.ini");
	GetPrivateProfileString("SMTP", "163", "", S163, 20, lpPath);
	delete[] lpPath;
	cout << "�������ռ�������" << endl;
	cin >> toWho;
	cout << "�������ʼ�����" << endl;
	cin >> theSubject;
	cout << "�������ʼ�����" << endl;
	cin >> theContent;



	Socket sock(S163, 25); //smtp.163.com 220.181.12.13
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	cout << "c:" << "helo xxxxx" << endl;
	sock.sendSocketMessage("helo xxxxx\r\n");
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	cout << "c:" << "auth login" << endl;
	sock.sendSocketMessage("auth login\r\n");
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	//�û��� ��Ҫbase64
	Base64Coder base64_obj;
	base64_obj.Encode(userName.data());//
	char* userBase64Buf = (char*)base64_obj.EncodedMessage();//base64ת��
	cout << "c:" << userName << endl;
	sock.sendSocketMessage(userBase64Buf);
	sock.sendSocketMessage("\r\n");
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl; //

//	Base64Coder base64_obj;
	base64_obj.Encode(passWord.data());//
	char* PasswordBase64Buf = (char*)base64_obj.EncodedMessage();//base64ת��
	cout << "c:" << passWord << endl;
	sock.sendSocketMessage(PasswordBase64Buf);
	sock.sendSocketMessage("\r\n");
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	cout << "*******************************" << endl;
	temp = "mail from: <" + userName + "@163.com>";
	cout << "c:" << temp << endl;
	sock.sendSocketMessage(temp.data());
	//sock.sendSocketMessage("xxxxx@163.com");
	//sock.sendSocketMessage(">");
	sock.sendSocketMessage("\r\n");
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	//�ռ���

	cout << "c:" << "rcpt to: <" << toWho << ">" << endl;
	sock.sendSocketMessage("rcpt to: <");
	sock.sendSocketMessage(toWho.data());
	sock.sendSocketMessage(">");
	sock.sendSocketMessage("\r\n");
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	//�ʼ�����
	cout << "c:" << "data" << endl;
	sock.sendSocketMessage("data\r\n");
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	cout << "*********************" << endl;
	cout << "c:" << "���͵��ʼ�����" << endl;

	//������
	temp = "from:" + userName + "@163.com\r\n";//temp��ʱ�����ַ���ƴ��
	sock.sendSocketMessage(temp.data());
	//�ռ���
	temp = "to:" + toWho + "\r\n";
	sock.sendSocketMessage(temp.data());
	//����
	temp = "subject:" + theSubject + "\r\n\r\n";
	sock.sendSocketMessage(temp.data());
	//����
	temp = theContent;
	sock.sendSocketMessage(temp.data());
	sock.sendSocketMessage("\r\n.\r\n");

	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;


	//�˳�
	cout << "c:" << "quit\r\n" << endl;
	sock.sendSocketMessage("quit\r\n");
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	sock.CloseSocket();
}

bool testSmtpAuthor_163(string userName, string passWord) {
	LPTSTR lpPath = new char[MAX_PATH];
	LPTSTR S163 = new char[20];
	strcpy(lpPath, "C:\\ip.ini");
	GetPrivateProfileString("SMTP", "163", "", S163, 20, lpPath);
	delete[] lpPath;
	Socket sock(S163, 25); //smtp.163.com
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	cout << "c:" << "helo xxxxx" << endl;
	sock.sendSocketMessage("helo xxxxx\r\n");
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	cout << "c:" << "auth login" << endl;
	sock.sendSocketMessage("auth login\r\n");
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	//�û��� ������Ҫbase64
	Base64Coder base64_obj;
	base64_obj.Encode(userName.data());
	char* userBase64Buf = (char*)base64_obj.EncodedMessage();//����Ϊbase64ת��
	cout << "c:" << userName << endl;
	sock.sendSocketMessage(userBase64Buf);
	sock.sendSocketMessage("\r\n");
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	//����(��Ȩ��) //ͬ��
//	Base64Coder base64_obj;
	base64_obj.Encode(passWord.data());
	char* PasswordBase64Buf = (char*)base64_obj.EncodedMessage();//����Ϊbase64ת��
	cout << "c:" << passWord << endl;
	sock.sendSocketMessage(PasswordBase64Buf);
	sock.sendSocketMessage("\r\n");
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	string test(sock.getReviceMessage());//��������
	int exist = test.find("235");
	if (exist == string::npos) {
		return false;
	}

	else {
		return true;
	}

}


void SMTP_qq(std::string userName, std::string passWord) {

	string toWho = "";//�ռ�������
	string theSubject = "";//���͵�����
	string theContent = "";//Ҫ���͵�����
	string temp = "";//��������ƴ�ӱ�׼Э����ַ���


	cout << "�������ռ�������" << endl;
	cin >> toWho;
	cout << "�������ʼ�����" << endl;
	cin >> theSubject;
	cout << "�������ʼ�����" << endl;
	cin >> theContent;


	LPTSTR lpPath = new char[MAX_PATH];
	LPTSTR SQQ = new char[20];
	strcpy(lpPath, "C:\\ip.ini");
	GetPrivateProfileString("SMTP", "qq", "", SQQ, 20, lpPath);
	delete[] lpPath;
	Socket sock(SQQ, 25);
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	cout << "c:" << "helo xxxxx" << endl;
	sock.sendSocketMessage("helo xxxxx\r\n");
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	cout << "c:" << "auth login" << endl;
	sock.sendSocketMessage("auth login\r\n");
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	//�û��� ������Ҫbase64
	Base64Coder base64_obj;
	base64_obj.Encode(userName.data());
	char* userBase64Buf = (char*)base64_obj.EncodedMessage();//����Ϊbase64ת��
	cout << "c:" << userName << endl;
	sock.sendSocketMessage(userBase64Buf);
	sock.sendSocketMessage("\r\n");
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	//����(��Ȩ��) //ͬ��
//	Base64Coder base64_obj;
	base64_obj.Encode(passWord.data());
	char* PasswordBase64Buf = (char*)base64_obj.EncodedMessage();//����Ϊbase64ת��
	cout << "c:" << passWord << endl;
	sock.sendSocketMessage(PasswordBase64Buf);
	sock.sendSocketMessage("\r\n");
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	cout << "*******************************" << endl;
	temp = "mail from: <" + userName + "@qq.com>";
	cout << "c:" << temp << endl;
	sock.sendSocketMessage(temp.data());
	//sock.sendSocketMessage("xxxxx@163.com");
	//sock.sendSocketMessage(">");
	sock.sendSocketMessage("\r\n");
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	//�ռ���

	cout << "c:" << "rcpt to: <" << toWho << ">" << endl;
	sock.sendSocketMessage("rcpt to: <");
	sock.sendSocketMessage(toWho.data());
	sock.sendSocketMessage(">");
	sock.sendSocketMessage("\r\n");
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	//�ʼ�����
	cout << "c:" << "data" << endl;
	sock.sendSocketMessage("data\r\n");
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	cout << "*********************" << endl;
	cout << "c:" << "���͵��ʼ�����" << endl;

	//������
	temp = "from:" + userName + "@qq.com\r\n";//temp��ʱ�����ַ���ƴ��
	sock.sendSocketMessage(temp.data());
	//�ռ���
	temp = "to:" + toWho + "\r\n";
	sock.sendSocketMessage(temp.data());
	//����
	temp = "subject:" + theSubject + "\r\n\r";
	sock.sendSocketMessage(temp.data());
	//����
	temp = theContent;
	sock.sendSocketMessage(temp.data());
	sock.sendSocketMessage("\r\n.\r\n");

	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;


	//�˳�
	cout << "c:" << "quit\r\n" << endl;
	sock.sendSocketMessage("quit\r\n");
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	sock.CloseSocket();
}


bool testSmtpAuthor_qq(std::string userName, std::string passWord) {

	LPTSTR lpPath = new char[MAX_PATH];
	LPTSTR SQQ = new char[20];
	strcpy(lpPath, "C:\\ip.ini");
	GetPrivateProfileString("SMTP", "qq", "", SQQ, 20, lpPath);
	delete[] lpPath;
	Socket sock(SQQ, 25); //SMTPЭ��ר�ö˿ں�
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	cout << "c:" << "helo xxxxx" << endl;
	sock.sendSocketMessage("helo xxxxx\r\n");
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	cout << "c:" << "auth login" << endl;
	sock.sendSocketMessage("auth login\r\n");
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	//�û��� ������Ҫbase64
	Base64Coder base64_obj;
	base64_obj.Encode(userName.data());
	char* userBase64Buf = (char*)base64_obj.EncodedMessage();//����Ϊbase64ת��
	cout << "c:" << userName << endl;
	sock.sendSocketMessage(userBase64Buf);
	sock.sendSocketMessage("\r\n");
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	//����(��Ȩ��) //ͬ��
//	Base64Coder base64_obj;
	base64_obj.Encode(passWord.data());//
	char* PasswordBase64Buf = (char*)base64_obj.EncodedMessage();//����Ϊbase64ת��
	cout << "c:" << passWord << endl;
	sock.sendSocketMessage(PasswordBase64Buf);
	sock.sendSocketMessage("\r\n");
	sock.reviceSocketMessage();
	cout << sock.getReviceMessage() << endl;

	string test(sock.getReviceMessage());//��������
	int exist = test.find("235");
	if (exist == string::npos) {
		return false;
	}

	else {
		return true;
	}
}