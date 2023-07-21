// ATM Solution.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include<fstream>
using namespace std;
struct PersonData
{
	string Account_Number, Pin_code, Name, PhoneNumber;
	double Account_Balance;
	bool Mark = false;
};
const string ClientsPath = "MyClients.txt";
PersonData LoginPerson;
enum enATM_Choice
{
	eQuickWithdraw = 1, eNormalWithdraw = 2, eDeposit, eCheckBalance, eLogout
};
void ATM();
void Login();
void NormalWithdraw();
int closest_5s_Mulitple(int Value);
void QuickWithdraw();
vector<string> split(string line, string dil) {
	vector<string> myvec;
	int pos;
	while ((pos = line.find(dil)) != line.npos) {
		string tmp = line.substr(0, pos);
		if (tmp != "")
			myvec.push_back(tmp);
		line.erase(0, pos + dil.length());
	}
	if (line != "")
		myvec.push_back(line);
	return myvec;
}
PersonData from_Line_to_Person(string Line) {
	vector<string> PersonInfo = split(Line, "#//#");
	return { PersonInfo[0],PersonInfo[1],PersonInfo[2],PersonInfo[3],stod(PersonInfo[4]) };

}
vector<PersonData> from_file_to_Vpersons() {
	fstream file;
	file.open(ClientsPath, ios::in);
	if (file.is_open())
	{
		vector < PersonData> Vpersons;
		string Line;
		while (getline(file, Line))
		{
			Vpersons.push_back(from_Line_to_Person(Line));
		}
		file.close();
		return Vpersons;
	}
	else {
		cout << "\n File doesn't exist [ Error in File ] \n ";
		return{ {"error"} };
	}
}

bool LoadLoginInfo(string Acc_Num, string Pin_Code, PersonData& Logininfo) {
	vector<PersonData> Vpersons = from_file_to_Vpersons();
	for (PersonData& Dummy : Vpersons) {
		if (Dummy.Account_Number == Acc_Num && Dummy.Pin_code == Pin_Code) {
			Logininfo = Dummy;
			return  true;
		}

	}
	return false;
}
bool AccountExist(string Acc_Num, string Pin_code) {
	return LoadLoginInfo(Acc_Num, Pin_code, LoginPerson);
}
void WhichScreen(string Choice) {
	cout << "=============================================";
	cout << endl << "\t\t" << Choice + " Screen"<<endl;
	cout << "============================================="<<endl;


}
void ShowAtmScreen() {
	WhichScreen("ATM Menu");
	cout << "\t\t[1] Quick Withdraw " << endl;
	cout << "\t\t[2] Normal Withdraw " << endl;
	cout << "\t\t[3] Deposit " << endl;
	cout << "\t\t[4] Check Balance " << endl;
	cout << "\t\t[5] Logout" << endl;
	cout << "=============================================" << endl;

}
enATM_Choice ReadATM_Choice() {
	cout << "\nPlease Enter What do you want to do from 1---->5 : ";
	int Number;
	cin >> Number;
	return enATM_Choice(Number);

}
void CheckBalance() {
	WhichScreen("Check Balance");
	cout << "Your Balance is : " << LoginPerson.Account_Balance<< "$ ";
}
void BackToATM() {
	cout << "\n\t\tPress Any Key To Go Bach to ATM !! ";
	system("pause>0");
	ATM();
}
double ReadDepositAmount() {
	double amount;

	do {
		cout << "Please enter positive deposite amount : ";
		cin >> amount;
		cout << endl;
	} while (amount <= 0);
		return amount;


	
}
void AddValuetoPersoninVpersons(const PersonData& Person, double Deposit_Amount, vector<PersonData> &Vpersons) {
	for (PersonData& Dummy : Vpersons) {
		if (Dummy.Account_Number == Person.Account_Number && Person.Pin_code == Dummy.Pin_code) {
			Dummy.Account_Balance += Deposit_Amount;
			return;

		}
	}
}
string From_Person_to_Line(const PersonData& Person, string Dill = "#//#") {
	return Person.Account_Number + Dill + Person.Pin_code + Dill + Person.Name + Dill + Person.PhoneNumber + Dill + to_string(Person.Account_Balance);
}
void from_Vpersons_to_file(const vector<PersonData>& Vpersons) {
	fstream file;
	file.open(ClientsPath, ios::out);
	if (file.is_open()) {
		for (const PersonData& Person : Vpersons)
			file << From_Person_to_Line(Person) << endl;
		file.close();
	}
	else {
		cout << "\n Error In File Handling !! " << endl;
	}
}
void PerformDeposit(PersonData &Person,double Deposit_Amount) {
	cout << "Are you Sure You want perform this transaction Y/N : ";
	char answer;
	cin >> answer;
	if (tolower(answer) != 'y')
		BackToATM();
	vector<PersonData> Vpersons = from_file_to_Vpersons();
	AddValuetoPersoninVpersons(Person, Deposit_Amount,Vpersons);
	from_Vpersons_to_file(Vpersons);
	Person.Account_Balance += Deposit_Amount;


}
void Deposit() {
	WhichScreen("Deposit");
	PerformDeposit(LoginPerson,ReadDepositAmount());
	cout << endl << "Done Succusfully :) " << "Your Balance now is : " << LoginPerson.Account_Balance<<endl;

}
short ReadQuickWithdrawChoice() {
	short Choice;

	do {
		cout << "Choose What do you want from 1---> 9 : ";
		cin >> Choice;
		cout << endl;
	} while (Choice > 9 || Choice < 0);
	return Choice;

}
void DrawQuickWithdrawScreen() {
	WhichScreen("Deposit");
	cout << "\t[1] 20\t\t[2] 50" << endl;
	cout << "\t[3] 100\t\t[4] 200" << endl;
	cout << "\t[5] 400\t\t[6] 600" << endl;
	cout << "\t[7] 800\t\t[8] 1000" << endl;
	cout << "\t[9] Exit"<<endl;
	cout << "============================================="<<endl;




}
double GetQuickWithdrawAmount(short Choice) {
	short WithdrawValue[9] = { 20,50,100,200,400,600,800,1000,0 };
	return WithdrawValue[Choice - 1];
}
void PerformQuickWithdraw(PersonData& WithdrawTarget, short WithdrawValue) {
	if (WithdrawValue == 0)
		ATM();
	if (WithdrawValue > WithdrawTarget.Account_Balance)
	{
		cout << " \t Wrong Value This Amount Exceeds Your Account \nBalance your able you withdraw up to : " << closest_5s_Mulitple(WithdrawTarget.Account_Balance) << "$";
		cout << "\n\t\tPress any key to try again ";
		system("pause>0");
		system("cls");
		QuickWithdraw();
		return;
	}
	PerformDeposit(WithdrawTarget, -1 * WithdrawValue);
	cout << "\n \tDone Succussfully :) " << "Your New Balance is : " << LoginPerson.Account_Balance << "$" << endl;
}
void QuickWithdraw() {
	DrawQuickWithdrawScreen();
	cout << "Your Balance is : " << LoginPerson.Account_Balance<<endl;
	PerformQuickWithdraw(LoginPerson, GetQuickWithdrawAmount(ReadQuickWithdrawChoice()));
}
int closest_5s_Mulitple(int Value) {
	while (Value%5!=0)
	{
		Value--;
	}
	return Value;
}
void PerformNormalWithdraw(PersonData &WithdrawTarget,double WithdrawAmount) {
	if (WithdrawAmount > WithdrawTarget.Account_Balance)
	{
		cout << " \t Wrong Value This Amount Exceeds Your Account \nBalance your able you withdraw up to : " << closest_5s_Mulitple(WithdrawTarget.Account_Balance) << "$";
		cout << "\n\t\tPress any key to try again ";
		system("pause>0");
		system("cls");
		NormalWithdraw();
		return;
	}
	PerformDeposit(WithdrawTarget, -1 * WithdrawAmount);
	cout << "\n\n \tWithdraw Done Seccussfully : ) " << "Your New Balance is:  " << LoginPerson.Account_Balance << "$";



}
double ReadNormalWithdrawAmount() {
	cout << "Please enter amount of withdraw in miltuple of 5s : ";
	double WithdrawValue;
	cin >> WithdrawValue;
	while (int(WithdrawValue)%5!=0)
	{
		cout << "\nWrong Value !! " << endl;
		cout << "Please enter amount of withdraw in miltuple of 5s : ";
		cin >> WithdrawValue;

	}
	cout << endl;
	return WithdrawValue;

}
void NormalWithdraw() {
	WhichScreen("Normal Withdraw");
	PerformNormalWithdraw(LoginPerson, ReadNormalWithdrawAmount());
}
void PerformATM(enATM_Choice Choice) {
	system("cls");
	switch (Choice)
	{
	case eQuickWithdraw:
		QuickWithdraw();
		BackToATM();

		break;
	case eNormalWithdraw:
		NormalWithdraw();
		BackToATM();
		break;
	case eDeposit:
		Deposit();
		BackToATM();
		break;
	case eCheckBalance:
		CheckBalance();
		BackToATM();

		break;
	case eLogout:
		Login();
		break;
	default:
		PerformATM(ReadATM_Choice());

		break;
	}
}
void ATM() {
	system("cls");
	ShowAtmScreen();
	PerformATM(ReadATM_Choice());

}
void Login() {
	WhichScreen("Login");
	bool isLoginFailed = false;
	



	do
	{
		if (isLoginFailed) {
			cout << "\n Login Failed !! Wrong Account Number/Pin Code" << endl<<endl;
		}
		string Account_Number;
		cout << "Please Enter Account Number : ";
		cin >> Account_Number;
		cout << endl;
		string Pin_Code;
		cout << "Please Enter Pin Code : ";
		cin >> Pin_Code;
		isLoginFailed = !AccountExist(Account_Number, Pin_Code);
	} while (isLoginFailed);

	ATM();
}
int main()
{

	Login();

}

