// bank.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include"stdafx.h"
#include<iostream>
#include<vector>
#include<map>
#include<cstring>
#include<fstream>
#include<sstream>
#include <string>
#include <winsock2.h>
#include<STDIO.h>
#include<ctype.h>
#include<time.h>
#include <thread>  
#include <Windows.h>  
#include <conio.h>
#include<sqlite3.h>
#include<time.h>
#include "json/json.h"
#include <iostream>
#include <fstream>
#include"class.hpp"

#pragma comment(lib,"json.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "sqlite3.lib") 

using namespace std;

int login(vector<Bank_card> &user,string &name)		//������½
{
	for (int i = 0; i < user.size(); i++)
	{
		if (user[i].equal(name))
		{	
			return i;
		}
	}
	cout << "��Ǹû���ҵ�" << endl;
	return -1;
}
int del_card(vector<Bank_card> &user,int &input_id,sqlite3 *db)
{
	char* sqsubstr = NULL;
	sqsubstr = sqlite3_mprintf("delete from bankcard where bankid = '%s'", user[input_id].getname().c_str());
	int rc = sqlite3_exec(db, sqsubstr, 0, 0, NULL);
	sqlite3_free(sqsubstr);

	vector<Bank_card>::iterator Iter;
	swap(user[input_id],user[user.size()-1]);
	user.pop_back();
	return 1;
}
int batchreg(sqlite3 *db,vector<Bank_card> &user)
{
	int flag = 0;
	string bankid = "";
	string password = "";
	string bankname = "";
	string id = "";
	string username = "";
	double balance = 0;
	ifstream ifle("USER.txt", ios::in);
	while (ifle.peek() != EOF)
	{
		ifle >> bankid;
		ifle >> password;
		ifle >> bankname;
		ifle >> id;
		ifle >> username;
		ifle >> balance;
		ifle.get();
		if (ifle.peek() == '/n')
			break;
		if (password.length() == 6 && id.length() == 8)
		{
			char* sqstr = NULL;
			string strbal = to_string(balance);
			sqstr = sqlite3_mprintf("INSERT INTO bankcard VALUES('%q','%q','%q','%q','%q','%q')", bankid.c_str(), password.c_str(), bankname.c_str(), id.c_str(), username.c_str(), strbal.c_str());
			int rc = sqlite3_exec(db, sqstr, 0, 0, NULL);
			if (rc != 0)
			{
				flag = 1;
			}
			else
			{
				Bank_card *b = new Bank_card;
				b->regbatch(bankid, password, bankname, id, username, balance);
				user.push_back(*b);
			}
			sqlite3_free(sqstr);
		}
		else
		{
			flag = 1;
		}
	}
	if (flag == 1)
	{
		cout << "�ļ���������" << endl;
	}
	else
	{
		cout << "����ע��ɹ�" << endl;
	}
	return 1;
}
int jsonreg(sqlite3 *db, vector<Bank_card> &user)
{
	int flag = 0;
	std::ifstream ifs;
	ifs.open("banker.json");

	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(ifs, root, false))
	{
		std::cout << "json open error" << std::endl;
		return 0;
	}
	Json::Value add_value = root["bank"];
	for (int i = 0; i < add_value.size(); ++i)
	{
		Json::Value temp_value = add_value[i];
		std::string bankid = temp_value["bankid"].asString();
		std::string password = temp_value["password"].asString();
		std::string bankname = temp_value["bankname"].asString();
		std::string id = temp_value["id"].asString();
		std::string username = temp_value["username"].asString();
		std::double_t balance = temp_value["balance"].asDouble();
		std::cout << "bankid:  " << bankid << " password: " << password << std::endl;

			// use value array[index]
			//Json::Value temp_value = add_value[i];
			//std::string strName = add_value[i]["name"].asString();
			//std::string strMail = add_value[i]["email"].asString();
			//std::cout << "name: " << strName << " email: " << strMail << std::endl;
		if (password.length() == 6 && id.length() == 8)
		{
			char* sqstr = NULL;
			string strbal = to_string(balance);
			sqstr = sqlite3_mprintf("INSERT INTO bankcard VALUES('%q','%q','%q','%q','%q','%q')", bankid.c_str(), password.c_str(), bankname.c_str(), id.c_str(), username.c_str(), strbal.c_str());
			int rc = sqlite3_exec(db, sqstr, 0, 0, NULL);
			if (rc != 0)
			{
				flag = 1;
			}
			else
			{
				Bank_card *b = new Bank_card;
				b->regbatch(bankid, password, bankname, id, username, balance);
				user.push_back(*b);
			}
			sqlite3_free(sqstr);
		}
		else
		{
			flag = 1;
		}
	}
		if (flag == 1)
		{
			cout << "josn�ļ�ע������" << endl;
			return 0;
		}
		else
		{
			cout << "json����ע��ɹ�" << endl;
			return 1;
		}
}
void initial(sqlite3 *db,vector<Bank_card> &user) {
	char *pErrMsg = 0;
	int ret = 0;
	string command = "";
	for (int i = 0; i < user.size(); i++)
	{
	command = user[i].retstr(db);
	ret = sqlite3_exec(db, command.c_str(), NULL, 0, &pErrMsg);
	if (ret != SQLITE_OK)
	{
	fprintf(stderr, "SQL insert error: %s\n", pErrMsg);
	sqlite3_free(pErrMsg); //���Ҫ��Ŷ��Ҫ��Ȼ���ڴ�й¶��Ŷ������
	sqlite3_close(db);
	return 1;
	}
	else
	cout << "���ݳ�ʼ���ɹ�" << endl;
	}
}
void thread01(vector<Bank_card>&user,sqlite3 *db)
{
	int sequence = -1;
	string keyword = "";
	string temp_name;
	string result = "";
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		cout << "�����쳣1" << endl;
	}

	//�����׽���
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		cout << "�����쳣2" << endl;
	}

	//��IP�Ͷ˿�
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	/*if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		cout << "�����쳣" << endl;
	}*/
	::bind(slisten, (LPSOCKADDR)&sin, sizeof(sin));
	//��ʼ����
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		cout << "�����쳣3" << endl;
	}

	//ѭ����������
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	char revData[255];
	while (true)
	{	
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			continue;
		}
		//	printf("���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(remoteAddr.sin_addr));

		//��������
		int ret = recv(sClient, revData, 255, 0);
		if (ret > 0)
		{
			revData[ret] = 0x00;
			string rece;
			rece = string(revData);
			string str1, str2, str3;
			istringstream is(rece);
			is >> str1 >> str2 >> str3;
			cout << str1 << endl;
			cout << str2 << endl;
			cout << "���տ���"<<str3 << endl;

			if (str1 == "��")
			{
				keyword = str3;
				sequence = login(user, str2);

				if (sequence == -1)
				{
					result = "��ʧ��";
					Sleep(1000);
					system("cls");
					cout << "---------------��ӭ��������---------------" << endl;
					cout << "	1.ע��  2.��½ 3.�˳�			" << endl;
				}
				else if (!user[sequence].judge_password(keyword))
				{
					result = "��ʧ��";
					Sleep(1000);
					system("cls");
					cout << "---------------��ӭ��������---------------" << endl;
					cout << "	1.ע��  2.��½ 3.�˳�			" << endl;
				}
				else
				{
					result = "�󶨳ɹ�";						
					Sleep(1000);
					system("cls");
					cout << "---------------��ӭ��������---------------" << endl;
					cout << "	1.ע��  2.��½ 3.�˳�			" << endl;
				}
				sequence = -1;
			}
			else
			{
				sequence = login(user, str1);
				keyword = str2;
				if (sequence == -1)
				{
					result = "��������";
					Sleep(1000);
					system("cls");
					cout << "---------------��ӭ��������---------------" << endl;
					cout << "	1.ע��  2.��½ 3.�˳�			" << endl;
				}
				else if (!user[sequence].judge_password(keyword))
				{
					result = "��������";
					Sleep(1000);
					system("cls");
					cout << "---------------��ӭ��������---------------" << endl;
					cout << "	1.ע��  2.��½ 3.�˳�			" << endl;
				}
				else
				{
					double money = atof(str3.c_str());
					if (user[sequence].withdraw(money))
					{

						result = "֧���ɹ�";
						time_t t;
						struct tm *ptr;
						t = time(NULL);
						ptr = gmtime(&t);
						char* sqstr = NULL;
						sqstr = sqlite3_mprintf("INSERT INTO history VALUES('%q','%q','withdraw','%q')", asctime(ptr), str1.c_str(), str3.c_str());
						int rc = sqlite3_exec(db, sqstr, 0, 0, NULL);
						sqlite3_free(sqstr);

						char * zSQL;
						zSQL = sqlite3_mprintf("update bankcard set balance = balance-'%q' where bankid = '%q'", str3.c_str(),str1.c_str());
						int rc1 = sqlite3_exec(db, zSQL, 0, 0, NULL);
						sqlite3_free(zSQL);

						Sleep(1000);
						system("cls");
						cout << "---------------��ӭ��������---------------" << endl;
						cout << "	1.ע��  2.��½ 3.�˳�			" << endl;
					}
					else
					{
						result = "֧��ʧ�� �˻�����";
						Sleep(1000);
						system("cls");
						cout << "---------------��ӭ��������---------------" << endl;
						cout << "	1.ע��  2.��½ 3.�˳�			" << endl;
					}
				}
			}
		}

		send(sClient, result.c_str(), strlen(result.c_str()), 0);
		closesocket(sClient);
	}

	closesocket(slisten);
	WSACleanup();
	ofstream ofle("1.txt", ios::out);    //app�ϲ������أ�
	for (int i = 0; i < user.size(); i++) {
		if (i == user.size() - 1) {
			user[i].outputend(ofle);
			ofle.close();
			break;
		}
		user[i].output(ofle);
	}
	ofle.close();
}
int main()
{

	sqlite3* db;
	int nResult = sqlite3_open("bank.db", &db);
	if (nResult != SQLITE_OK)
	{
		cout << "�����ݿ�ʧ�ܣ�" << sqlite3_errmsg(db) << endl;
		return 0;
	}
	else
	{
		cout << "�������ݿ�򿪳ɹ�" << endl;
	}

	//���ݿ�
	vector<Bank_card> user;
	ifstream ifle("1.txt", ios::in);
	while (!ifle.eof())
	{
		Bank_card *a = new Bank_card;
		a->input(ifle);
		user.push_back(*a);
	}

	char *pErrMsg = 0;
	int ret = 0;
	string command = "";


	thread task01(thread01,user,db);
	task01.detach();
	while (1)
	{
		cout << "---------------��ӭ��������---------------" << endl;
L3:		cout << "	1.ע��  2.��½ 3.�˳�			"<<endl;
		string strnum;
		cin >> strnum;
		int num = atoi(strnum.c_str());

		if (num == 1) {
			cout << "1����ͨע�� 2������ע��" << endl;
			string strcho;
			cin >> strcho;
			int cho = atoi(strcho.c_str());
			if (cho == 1)
			{
				string cho_bank;
				cho_bank = choose_bank();
				Bank_card *b = inforconstruct(cho_bank,db);
				user.push_back(*b);
				b->show();
			}
			else if (cho == 2)
			{
				//batchreg(db,user);				//���ַ�ʽ
				jsonreg(db, user);
			}
			else {
				cout << "�������" << endl;
				goto L3;
			}
		}
		else if (num == 2) {

			string keyword;
			string temp_name = "";
			cout << "���������п���" << endl;
			cin >> temp_name;
		RE:	cout << "����������" << endl;
			cin >> keyword;
			if (keyword.length() != 6)
			{
				cout << "��������λ��λ������������" << endl;
				keyword = "";
				goto RE;
			}
			int sequence = login(user, temp_name);			//����λ��

			if (sequence == -1 )
			{	
				cout << "�˺Ż���������" << endl;
				goto L3;
			}
			else if (!user[sequence].judge_password(keyword))
			{
				cout << "�˺Ż���������" << endl;
				goto L3;
			}
			while (1) {
				cout << "�����Ƿ�����޸� 1.�޸����� 2.��� 3.��� 4.���� 5.�鿴��Ϣ 6.����" << endl;
				int choose;
				cin >> choose;
				if (choose == 1) {
					user[sequence].change(db);
				}
				else if (choose == 2) {
					cout << "��������" << endl;
					double account;
					cin >> account;
					int flag = 0;
					if (user[sequence].withdraw(account))
						flag = 1;
					user[sequence].show();
					if (flag == 1)
					{
						time_t t;
						struct tm *ptr;
						t = time(NULL);
						ptr = gmtime(&t);
						char* sqstr = NULL;
						string stracc = to_string(account);
						sqstr = sqlite3_mprintf("INSERT INTO history VALUES('%q','%q','withdraw','%q')", asctime(ptr), temp_name.c_str(), stracc.c_str());
						int rc = sqlite3_exec(db, sqstr, 0, 0, NULL);
						sqlite3_free(sqstr);

						char * zSQL;
						zSQL = sqlite3_mprintf("update bankcard set balance = balance-'%q' where bankid = '%q'", stracc.c_str(),temp_name.c_str());
						int rc1 = sqlite3_exec(db, zSQL, 0, 0, NULL);
						sqlite3_free(zSQL);
					}
				}
				else if (choose == 3) {
					cout << "��������" << endl;
					double account;
					cin >> account;
					user[sequence].deposit(account);
					user[sequence].show();
					struct tm *ptr;
					time_t t;
					t = time(NULL);
					ptr = gmtime(&t);
					char* sqstr = NULL;
					string stracc = to_string(account);
					sqstr = sqlite3_mprintf("INSERT INTO history VALUES('%q','%q','deposit','%q')", asctime(ptr),temp_name.c_str(),stracc.c_str());
					int rc = sqlite3_exec(db, sqstr, 0, 0, NULL);
					sqlite3_free(sqstr);

					char * zSQL;
					zSQL = sqlite3_mprintf("update bankcard set balance = balance+'%q' where bankid = '%q'",stracc.c_str(),temp_name.c_str());
					int rc1 = sqlite3_exec(db, zSQL, 0, 0, NULL);
					sqlite3_free(zSQL);
				}
				else if (choose == 4)
				{
					if (del_card(user, sequence,db))
					{
						cout << "�����ɹ�" << endl;
					}
					else {
						cout << "����ʧ��" << endl;
					}
				}
				else if (choose == 5)
					user[sequence].show();
				else if (choose == 6)
					break;
			}
		}
		else if (num == 3)
		{
			ifle.close();
			break;
		}
		else
			goto L3;
	}
	ofstream ofle("1.txt", ios::out);    //app�ϲ������أ�
	for (int i = 0; i < user.size(); i++) {
		if (i == user.size() - 1) {
			user[i].outputend(ofle);
			ofle.close();
			break;
		}
		user[i].output(ofle);
	}
	ofle.close();
	return 0;
}
