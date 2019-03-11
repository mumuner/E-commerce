// client.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <WINSOCK2.H>
#include <STDIO.H>
#include<iostream>
#include<string.h>
#include<string>


#pragma  comment(lib,"ws2_32.lib")

using namespace std;

int main(int argc, char* argv[])
{
	string bank_id;
	string bank_password;
	
	cin >> bank_id;
	cin >> bank_password;

	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return 0;
	}

	SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sclient == INVALID_SOCKET)
	{
		cout << "invalid socket !" << endl;
		return 0;
	}

	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(8888);
	serAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		cout << "connect error !" << endl;;
		closesocket(sclient);
		return 0;
	}
	//char * sendData = "你好，TCP服务端，我是客户端!\n";

	string id_pass = bank_id + " " + bank_password;
	const char* id_password = id_pass.data();
	send(sclient, id_password, strlen(id_password),0);

	char recData[255];
	int ret = recv(sclient, recData, 255, 0);
	if (ret > 0)
	{
		recData[ret] = 0x00;
		cout <<recData ;
	}
	closesocket(sclient);
	WSACleanup();
	return 0;
}

