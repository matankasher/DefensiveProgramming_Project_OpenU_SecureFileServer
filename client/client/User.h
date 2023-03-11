#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <WS2tcpip.h>
#include <sstream>
#include <Windows.h>
#include <fstream>
#include <pem.h>
#include <osrng.h>
#include <rsa.h>
#define _CRT_SECURE_NO_WARNINGS
using namespace std;
using namespace CryptoPP;
class User   // contain all user information and his keys
{
public:
	static const unsigned int DEFAULT_KEYLENGTH = 16; // 128 byte
	string name ;
	string FileName;
	InvertibleRSAFunction params;
	CryptoPP::AutoSeededRandomPool _rng;
	RSA::PrivateKey priKey;
	RSA::PublicKey pubKey;
	boolean newUser;
	unsigned char _key[DEFAULT_KEYLENGTH];
	char clientID [16];
	User()
	{
		getName(1, "transfer.info");
		getName(2, "transfer.info");
		ifstream f("me.info");
		if (!f.good())
			newUser = true;
		else
			newUser = false;
	}
	void getName(int i,string fileName);
	boolean isNewUser();
	string getFileContent();
};

