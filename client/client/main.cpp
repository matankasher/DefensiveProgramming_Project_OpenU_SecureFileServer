#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <WS2tcpip.h>
#include <sstream>
#include <Windows.h>
#include "wsa.h"
#include "ConncetParameters.h"
#include "requestHandler.h"
#pragma comment(lib, "ws2_32.lib")
#define client_version 3
using namespace std;
WSA wsa;
ConnectParameters conP;

// © All right reserved to Matan Kasher 2023A


int main()
{
	cout << "Welcome to saving files servers \n";
	try
	{
		 ConnectParameters conP; // contain the connect parameters to the server
	}
	catch (const std::exception& e)
	{
		cout << "Error: " << e.what() << endl;
		return 0;
	}
	cout << "connect with the address : " << conP.address << " and the port: " << conP.port << endl;
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
		WSACleanup();
		return 0;
	}
	sockaddr_in hint = { 0 };
	hint.sin_family = AF_INET;
	hint.sin_addr.s_addr = inet_addr(conP.address.c_str());
	hint.sin_port = htons(conP.port);
	int connResult = connect(sock, (struct sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
		closesocket(sock);
	}
	User* u = new User();
	requestHandler(&sock, u);  // start to handel the client requests.
	closesocket(sock);
	WSACleanup();
}
