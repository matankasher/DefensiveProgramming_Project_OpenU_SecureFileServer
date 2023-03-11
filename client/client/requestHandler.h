#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <WS2tcpip.h>
#include <sstream>
#include <Windows.h>
#include "Encryption.h"
#include "request.h"
#include "respone.h"
#include "aes.h"
#include "modes.h"


using namespace std;
using namespace CryptoPP;

void sendPublicKeyRequest(SOCKET* sock, User* u);
void registrationResponse(SOCKET* sock, User* u);
void RegistrationRequest(SOCKET* sock, User* u);
void ReconncetRequest(SOCKET* sock, User* u);
int  requestHandler(SOCKET* sock, User* u);
void sendAESkeyResponse(SOCKET* sock, User* u);