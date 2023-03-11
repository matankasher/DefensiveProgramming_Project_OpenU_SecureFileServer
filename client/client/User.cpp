#include "User.h"
#include <pem.h>
#define _CRT_SECURE_NO_WARNINGS
void  User::getName(int i, string fileName)
{
	string name1 = "";
	ifstream ReadPort(fileName);
	getline(ReadPort, name1);
	if (i == 1)  // get client name
	{
		getline(ReadPort, name);
	}
	if (i == 2) //  get file name we need to send.
	{
		getline(ReadPort, name1);
		getline(ReadPort, FileName);
	}
}
boolean User::isNewUser()
{
	return newUser;
}
string User::getFileContent()
{
	ifstream f(FileName);
	if(!f.good())
	{
		string s = "\n can't open the file to send";
		throw(runtime_error(s));
	}
	stringstream strStream;
	strStream << f.rdbuf();
	return strStream.str();
}
 
