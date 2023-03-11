#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <WS2tcpip.h>
#include <sstream>
#include <Windows.h>
#include "rsa.h"
using namespace std;
class ConnectParameters
{
public:
	int port;
	string address;
	ConnectParameters()
	{
		try
		{
			port = stoi(readPortaddress(1)); // Listening port # on the server.
			// the address of the client
		}
		catch (const std::exception& e)
		{
			string s = "\n can't open the transfer.info file";
			throw(runtime_error(s));
		}
		address = readPortaddress(2);
	}
	ConnectParameters(int i)
	{
		port = 0;
		address = "";
	}
	~ConnectParameters()
	{

	}
	std::string readPortaddress(int i);

};

