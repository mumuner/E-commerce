#include"stdafx.h"
#include <string>
#include<iostream>
#include "json/json.h"
#include <iostream>
#include <fstream>
#include<vector>
#include<sqlite3.h>
using namespace std;
#pragma comment(lib,"json.lib")
/*void jsonreg(sqlite3 *db, vector<Bank_card> &user)
{
	std::ifstream ifs;
	ifs.open("banker.json");

	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(ifs, root, false))
	{
		std::cout << "json open error" << std::endl;
		return;
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
	}
	system("Pause");
}*/