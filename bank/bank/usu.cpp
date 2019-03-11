#include<vector>
#include"class.hpp"
#include<iostream>
#include<fstream>
#include<sstream>
#include<iostream>
#include <conio.h>
#include<sqlite3.h>
#include "json/json.h"
#pragma comment(lib,"json.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "sqlite3.lib") 

using namespace std;
int login(std:: vector<Bank_card> &user, std::string &name)		//用来登陆
{
	for (int i = 0; i < user.size(); i++)
	{
		if (user[i].equal(name))
		{
			return i;
		}
	}
	std::cout << "抱歉没有找到" << std::endl;
	return -1;
}
int del_card(std:: vector<Bank_card> &user, int &input_id, sqlite3 *db)
{
	char* sqsubstr = NULL;
	sqsubstr = sqlite3_mprintf("delete from bankcard where bankid = '%s'", user[input_id].getname().c_str());
	int rc = sqlite3_exec(db, sqsubstr, 0, 0, NULL);
	sqlite3_free(sqsubstr);

	std:: vector<Bank_card>::iterator Iter;
	std::swap(user[input_id], user[user.size() - 1]);
	user.pop_back();
	return 1;
}
int batchreg(sqlite3 *db, std:: vector<Bank_card> &user)
{
	int flag = 0;
	std::string bankid = "";
	std::string password = "";
	std::string bankname = "";
	std::string id = "";
	std::string username = "";
	double balance = 0;
	std::ifstream ifle("USER.txt", ios::in);
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
			std::string strbal = std::to_string(balance);
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
		std::cout << "文件部分有误" << std::endl;
	}
	else
	{
		std::cout << "批量注册成功" << std::endl;
	}
	return 1;
}
int jsonreg(sqlite3 *db, std:: vector<Bank_card> &user)
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
		//std::std::string strName = add_value[i]["name"].asString();
		//std::std::string strMail = add_value[i]["email"].asString();
		//std::std::cout << "name: " << strName << " email: " << strMail << std::std::endl;
		if (password.length() == 6 && id.length() == 8)
		{
			char* sqstr = NULL;
			std::string strbal = to_string(balance);
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
		std::cout << "josn文件注册有误" << std::endl;
		return 0;
	}
	else
	{
		std::cout << "json批量注册成功" << std::endl;
		return 1;
	}
}
void initial(sqlite3 *db, std:: vector<Bank_card> &user) {
	char *pErrMsg = 0;
	int ret = 0;
	std::string command = "";
	for (int i = 0; i < user.size(); i++)
	{
		command = user[i].retstr(db);
		ret = sqlite3_exec(db, command.c_str(), NULL, 0, &pErrMsg);
		if (ret != SQLITE_OK)
		{
			fprintf(stderr, "SQL insert error: %s\n", pErrMsg);
			sqlite3_free(pErrMsg); //这个要的哦，要不然会内存泄露的哦！！！
			sqlite3_close(db);
			return 1;
		}
		else
			std::cout << "数据初始化成功" << std::endl;
	}
}