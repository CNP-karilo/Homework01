#include<iostream>
#include<string>
#include"POP3.h"
#include"SMTP.h"
#include <windows.h>
#include<Winbase.h>
using namespace std;
int main() {
	bool ifQuitArgent = false;//�����Ƿ��˳���������
	int qqOr163 = 0; //qq��163�����ѡ��
	int smtpOrPop3 = 0;//smtp��pop3��ѡ��

	string userName = "";//�û���
	string passWord = "";//����
	LPTSTR lpPath = new char[MAX_PATH];
	LPTSTR UserName = new char[20];
	LPTSTR PassWord = new char[20];
	strcpy(lpPath, "C:\\ip.ini");
	GetPrivateProfileString("user", "userName", "", UserName, 20, lpPath);
	GetPrivateProfileString("user", "passWord", "", PassWord, 20, lpPath);
	delete[] lpPath;
	cout << "��ӭ�����ʼ�����ϵͳ" << endl;
	cout << "֧��163�����qq����Ĵ���" << endl;

	while (qqOr163 != 4) {

		cout << "1.ʹ��qq����������" << endl;
		cout << "2.ʹ��163����������(Ϊ@163.comϵ��)" << endl;
		cout << "3.ʹ�ô���󶨵�QQ�������(�˴���ư�ȫ���⣬δ���������˺�����)" << endl;
		cout << "4.�Ƴ��ô�ʹ�ã��رճ���" << endl;
		cin >> qqOr163;
		system("cls");

		switch (qqOr163)
		{
		case 1:
			cout << "��������qq�ʼ�����ϵͳ" << endl;
			cout << "�������û���(������@xxx�Ĳ��֣�ֻ����@֮ǰ������)" << endl;
			cin >> userName;
			cout << "������QQsmtp/pop3Э�����Ȩ����Ϊ���루��Ҫ�ֶ���ͨ�����smtp/pop3Э��õ���Ȩ��)" << endl;
			cin >> passWord;
			if (!testSmtpAuthor_qq(userName, passWord)) {//�û��������벻��ȷ
				cout << "�û��������벻��ȷ" << endl;
				system("pause");
				system("cls");

				break;
			}
			else {
				system("cls");
				cout << "1.ʹ��SMTPдQQ�ʼ��ķ���" << endl;
				cout << "2.ʹ��POP3����QQ�ʼ������صķ���" << endl;
				cout << "3.�˳�QQ����������" << endl;
				cin >> smtpOrPop3;
				while (smtpOrPop3 != 3) {

					if (smtpOrPop3 == 1) {
						SMTP_qq(userName, passWord);
						system("pause");
						system("cls");
					}
					else if (smtpOrPop3 == 2) {
						POP3_qq(userName, passWord);
						system("pause");
						system("cls");
					}
					else if (smtpOrPop3 == 3) {
					}
					else
					{
						cout << "ֻ������1��2��3" << endl;
					}
				}
			}
			break;
		case 2:
			cout << "��������163�ʼ�����ϵͳ" << endl;
			cout << "�������û���(������@xxx�Ĳ��֣�ֻ����@֮ǰ������)" << endl;
			cin >> userName;
			cout << "������163smtp/pop3Э�����Ȩ����Ϊ���루��Ҫ�ֶ���ͨ�����smtp/pop3Э��õ���Ȩ��)" << endl;
			cin >> passWord;
			if (!testSmtpAuthor_163(userName, passWord)) {//�û��������벻��ȷ
				cout << "�û��������벻��ȷ" << endl;
				system("pause");
				system("cls");
				break;
			}
			else {
				system("cls");
				cout << "1.ʹ��SMTPд163�ʼ��ķ���" << endl;
				cout << "2.ʹ��POP3����163�ʼ������صķ���" << endl;
				cout << "3.�˳�163����������" << endl;
				cin >> smtpOrPop3;
				while (smtpOrPop3 != 3) {
					if (smtpOrPop3 == 1) {
						SMTP_163(userName, passWord);
						system("pause");
						system("cls");
					}
					else if (smtpOrPop3 == 2) {
						POP3_163(userName, passWord);
						system("pause");
						system("cls");
					}
					else if (smtpOrPop3 == 3) {
					}
					else
					{
						cout << "ֻ������1��2��3" << endl;
					}
				}
			}


			break;
		case 3:
			cout << "ʹ�ô���󶨵�QQ�������ϵͳ" << endl;
			cout << "1.����SMTPд�ʼ�����" << endl;
			cout << "2.����pop3�����������" << endl;
			cout << "3.�˳�" << endl;
			cin >> smtpOrPop3;
			while (smtpOrPop3 != 3) {
				if (smtpOrPop3 == 1) {

					SMTP_qq(UserName, PassWord);
					system("pause");
					system("cls");
				}
				else if (smtpOrPop3 == 2) {

					POP3_qq(UserName, PassWord);
					system("pause");
					system("cls");
				}
				else if (smtpOrPop3 == 3) {
				}
				else
				{
					cout << "ֻ������1,2,3" << endl;
				}
			}
			break;
		case 4:
			system("cls");
			break;
		default:
			cout << "������1��2��3��4" << endl;
			break;
		}
	}
}