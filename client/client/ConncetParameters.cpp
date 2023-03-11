#include "ConncetParameters.h"
#include <iostream>
#include <fstream>
#include <string>
#include <WS2tcpip.h>
#include <sstream>
#include <Windows.h>
using namespace std;

std::string ConnectParameters::readPortaddress(int i)
{
	string portAddressLine = "";
	ifstream ReadPort("transfer.info");
	if (ReadPort)
	{
		getline(ReadPort, portAddressLine);
		istringstream iss(portAddressLine);
		std::getline(iss, portAddressLine, ':');
		if (i == 2) // return the address
		{
			ReadPort.close();
			return (portAddressLine);
		}
		std::getline(iss, portAddressLine, '\n');
		ReadPort.close();
		return (portAddressLine); // return the port
	}
	else
	{
		string s = "\n can't open the transfer.info file";
		throw(runtime_error(s));
	}
}