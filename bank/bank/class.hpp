#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<sqlite3.h>
#include<time.h>
class Bank_card
{
private:
	std:: string cardid = "";
	std:: string password = "";
	std:: string bankname = "";
	std:: string id = "";
	std:: string name = "";
	double balance = 0;
public:

	void reg(std:: string &inputed, std:: string &person, std:: string &inputpassword, std:: string &personid, sqlite3* db); //ע��
	void Bank_card::regbatch(std:: string &bank_id, std:: string &inputpassword, std:: string &inputbankname, std:: string &user_id, std:: string user_name, double inputbal);
	void change(sqlite3 *db);	//�޸�����
	void deposit(double &num);		//���	
	int withdraw(double &num);	//ȡ��
	void show();		//��ʾ��Ϣ
	void output(std::ofstream &ofle);//������ļ�
	void outputend(std::ofstream &ofle);//ĩβ������ļ�
	void input(std::ifstream &ifle);//���ļ�����
	int equal(std:: string idbank);//�����ж���Ӧ����
	std:: string getname();
	inline int judge_password(std:: string &input_pass)
	{
		return password == input_pass ? 1 : 0;
	}
	inline int Bank_card::judge_id(std:: string &input_id)
	{
		return cardid == input_id ? 1 : 0;
	}
	std:: string retstr(sqlite3 *db)
	{
		std:: string strbal = std::to_string(balance);
		return sqlite3_mprintf("INSERT INTO bankcard VALUES('%q','%q','%q','%q','%q','%q')", cardid.c_str(), password.c_str(), bankname.c_str(),
			id.c_str(), name.c_str(), strbal.c_str());
	}
};
std:: string Bank_card::getname()
{
	return cardid;
}
void Bank_card::reg(std:: string &inputed, std:: string &person, std:: string &inputpassword, std:: string &personid, sqlite3*db)
{
	bankname = inputed;
	name = person;
	password = inputpassword;
	id = personid;
	srand((unsigned)time(NULL));
	cardid = std::to_string(rand() % 100000 + 100000);
	balance = 0;
	inputed = "";
	char* sqstr = NULL;
	std:: string strbal = std::to_string(balance);
	sqstr = sqlite3_mprintf("INSERT INTO bankcard VALUES('%q','%q','%q','%q','%q','%q')", cardid.c_str(), password.c_str(), bankname.c_str(), id.c_str(), name.c_str(), strbal.c_str());
	int rc = sqlite3_exec(db, sqstr, 0, 0, NULL);
}
void Bank_card::regbatch(std:: string &bank_id, std:: string &inputpassword, std:: string &inputbankname, std:: string &user_id, std:: string user_name, double inputbal)
{
	bankname = inputbankname;
	name = user_name;
	password = inputpassword;
	id = user_id;;
	cardid = bank_id;
	balance = inputbal;
}
void Bank_card::change(sqlite3 *db)
{
	std:: string inputpassword;
L1:	 std::cout << "����������" << std::endl;
	std::cin >> inputpassword;			//��һ������
	std:: string inputpassword2;
	 std::cout << "���ٴ���������" << std::endl;
	std::cin >> inputpassword2;
	while (1)
	{
		if (inputpassword == inputpassword2) {
			 std::cout << "�Ƿ�ȷ���޸����� ?    Y or N" << std::endl;
			char assure;
			std::cin >> assure;
			if (assure == 'Y') {
				 std::cout << "������ȷ��" << std::endl;
				char * zSQL;
				zSQL = sqlite3_mprintf("update bankcard set password = '%q' where bankid = '%q'", inputpassword2.c_str(), cardid.c_str());
				int rc = sqlite3_exec(db, zSQL, 0, 0, NULL);
				sqlite3_free(zSQL);
				break;
			}
			else {
				goto L1;
			}
		}
		else
		{
			 std::cout << "�����������������" << std::endl;
			std::cin >> inputpassword2;
		}
	}
	password = inputpassword2;
}
void Bank_card::deposit(double &num)
{
	if (num < 0)
	{
		 std::cout << "������������" << std::endl;
		return;
	}
	balance += num;
	 std::cout << "Now,your balance is :" << balance << std::endl;
}
int Bank_card::withdraw(double &num)
{
	if (num > balance)
	{
		 std::cout << "Error, your balance is insufficient !" << std::endl;
		return 0;
	}
	else if (num <= 0)
	{
		 std::cout << "������������" << std::endl;
		return 0;
	}
	else
	{
		balance -= num;
		 std::cout << "Now,your balance is :" << balance << std::endl;
		return 1;
	}
}
void Bank_card::show()
{
	 std::cout << " Welcome to the " << bankname << std::endl;
	 std::cout << "Card Id :" << cardid << std::endl;
	 std::cout << "Name :" << name << std::endl;
	 std::cout << "Balance:" << balance << std::endl;
}
std:: string choose_bank()				//ѡ�����У����п��������������� ���߿��԰����Ͷ���Ϊstd:: string���ͣ��������е�����
{
L2:	std:: string bank_name;
	std::ifstream ifle("bank.txt", std::ios::in);
	int flag = 0;
	char word;
	 std::cout << "��ѡ������" << std::endl;
	std:: string bank, inputbankname;
	std::cin >> bank;
	inputbankname = "";
	word = ifle.get();
	while (word != EOF)
	{
		while (word != '\n'&&word != EOF)
		{
			inputbankname += word;
			word = ifle.get();
		}
		if (inputbankname == bank) {
			 std::cout << "the bank is in it" << std::endl;
			bank_name = bank;
			flag = 1;
			ifle.close();
			return bank;
		}
		else
		{
			inputbankname = "";
			word = ifle.get();
		}
	}
	if (flag == 0)
	{
		 std::cout << "sorry , your input is unprecise,please input again " << std::endl;
		ifle.close();
		goto L2;
	}
}
Bank_card *inforconstruct(std:: string &bankname, sqlite3* db)			//�����п���ע����Ϣ����
{
	Bank_card *a = new Bank_card;				//���ⲿ�ֽ����޸�
	std:: string person;
	std:: string inputpassword;
	std:: string personid;
	 std::cout << "�������������" << std::endl;
	std::cin >> person;
L1:	 std::cout << "�������������" << std::endl;			//��һ������
CODE:
	std::cin >> inputpassword;
	if (inputpassword.length() != 6)
	{
		inputpassword = "";
		 std::cout << "��������λ����" << std::endl;
		goto CODE;
	}
	 std::cout << "���ٴ���������" << std::endl;
	std:: string inputpassword2;
	std::cin >> inputpassword2;
	while (1)
	{
		if (inputpassword == inputpassword2) {
			 std::cout << "�Ƿ�ȷ��    Y " << std::endl;
			char assure;
			std::cin >> assure;
			if (assure == 'Y') {
				 std::cout << "����ȷ��" << std::endl;
				break;
			}
			else {
				goto L1;
			}
		}
		else
		{
			 std::cout << "������������������" << std::endl;
			std::cin >> inputpassword2;
		}
	}
	 std::cout << "�������������֤��:" << std::endl;
ID:	std::cin >> personid;
	if (personid.length() != 8)
	{

		 std::cout << "�������λ���֤��" << std::endl;
		personid = "";
		goto ID;
	}
	a->reg(bankname, person, inputpassword, personid, db);
	return a;
}
void Bank_card::output(std::ofstream &ofle)
{
	ofle << cardid;
	ofle << '\n';
	ofle << password;
	ofle << '\n';
	ofle << bankname;
	ofle << '\n';
	ofle << id;
	ofle << '\n';
	ofle << name;
	ofle << '\n';
	ofle << balance;
	ofle << '\n';
}
void Bank_card::outputend(std::ofstream &ofle)
{
	ofle << cardid;
	ofle << '\n';
	ofle << password;
	ofle << '\n';
	ofle << bankname;
	ofle << '\n';
	ofle << id;
	ofle << '\n';
	ofle << name;
	ofle << '\n';
	ofle << balance;
}
void Bank_card::input(std::ifstream &ifle)
{
	ifle >> cardid;
	ifle >> password;
	ifle >> bankname;
	ifle >> id;
	ifle >> name;
	ifle >> balance;
}
int Bank_card::equal(std:: string idbank)
{
	if (idbank == cardid)
		return 1;
	else
		return 0;
