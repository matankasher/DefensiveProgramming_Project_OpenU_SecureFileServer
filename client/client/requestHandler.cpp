#include "requestHandler.h"


void successCRC(SOCKET* sock, User* u)
{
	ValidCRC pack;
	RequestHeader header;
	ValidCRCPayload payload;
	header.code = ValidCRCRequestCode;
	strcpy(header.client_id, u->clientID);
	strcpy(payload.fileName, u->FileName.c_str());
	header.payload_size = sizeof(payload);
	pack = { header ,payload };
	send(*sock, (char*)&pack, sizeof(pack), 0);
	ComfirmRecieveMessageResponse respone;
	recv(*sock, (char*)&respone.header, sizeof(respone.header), 0);
	recv(*sock, (char*)&respone.payload, respone.header.payload_size, 0);
	if (respone.header.code == ConfirmReceivedMessageResponseCode)
		cout << "Successed to Saved the file at the server.";
	else
		cout << "couldn't save the file at the server , please try again.";
}

void CRCHandler(SOCKET* sock, User* u, string fileContent)
{
	int i = 0;
	while (i < 4)  // tring to verify CRC up to 3 attempts
	{
		RecieveValidFileWithCRCResponse respone;
		recv(*sock, (char*)&respone.header, sizeof(respone.header), 0);
		recv(*sock, (char*)&respone.payload, respone.header.payload_size, 0);
		if (Encryption::CheckCRC(fileContent, respone.payload.cksum))
			successCRC(sock, u);
		InvalidCRC pack;
		RequestHeader header;
		InvalidCRCPayload payload;
		strcpy(header.client_id, u->clientID);
		if (i == 3)
			header.code = FourthInvalidCRCRequestCode;
		else
		header.code = InvalidCRCRequestCode;
		strcpy(payload.fileName, fileContent.c_str());
		header.payload_size = sizeof(payload);
		pack = { header ,payload };
		send(*sock, (char*)&pack, sizeof(pack), 0);
		i++;
	}
}

void SendFileRequest(SOCKET* sock, User* u) // handle encrypt and send file
{
	string fileContent;
	try
	{
		fileContent = u->getFileContent();
	}
	catch (const std::exception& e)
	{
		cout << "Error: " << e.what() << endl;
		return;
	}
	SendFile pack;
	RequestHeader rHeader;
	SendFileRequestPayload payload; 
	rHeader.code = SendFileRequestCode;
	strcpy(rHeader.client_id, u->clientID);
	for (int i = 0; i < 255; i++)  // declare arr.
		payload.fileName[i] = '\0';
	strcpy(payload.fileName, u->FileName.c_str());
	string encryptContent = Encryption::AESencrypt(fileContent.c_str(), fileContent.length(), u); // encrypt the content
	payload.content_size = sizeof(encryptContent);  
	rHeader.payload_size = sizeof(payload);
	pack = { rHeader, payload };
	send(*sock, (char*)&pack, sizeof(pack), 0);
	send(*sock, encryptContent.c_str(), encryptContent.length(), 0);  // send the encrypt file content
	CRCHandler(sock, u, fileContent);
}



void sendAESkeyOrReconnectResponse(SOCKET* sock, User* u) //2102 or 2105: Succuss Register/Reconnect Response 
{
	SendAESkeyResponse respone;
	recv(*sock, (char*)&respone.header, sizeof(respone.header), 0);
	recv(*sock, (char*)&respone.payload, respone.header.payload_size, 0);
	if (respone.header.code == RequestToReconncetDeniedResponseCode)
		RegistrationRequest(sock , u);
	if (respone.header.code != SendAESkeyResponseCode && respone.header.code != ConfirmRequestToReconncetResponseCode)
		return;
	string AESkey(respone.payload.encryptedAESkey, respone.header.payload_size -16);
	try
	{   
		AESkey = Encryption::decrypt(AESkey, u);   // decrypt the aes key with private key
		Encryption::AESWrapper(AESkey.c_str(), AESkey.length(), u);  
	}
	catch (const std::exception&)
	{
		cout << "can't able to decrypt AES key " << endl;
		return;
	}
	SendFileRequest(sock , u);
}




void sendPublicKeyRequest(SOCKET* sock, User* u)
{
	RequestHeader rHeader;
	rHeader.code = SendpublicKeyRequestCode;
	SendpublicKeyPayload publicKeyPL;
	strcpy(rHeader.client_id, u->clientID);
	strcpy(publicKeyPL.name, u->clientID);
	strcpy(publicKeyPL.publicKey, Encryption::printPublicKey(u->pubKey).c_str());
	rHeader.payload_size = sizeof(publicKeyPL);
	SendpublicKey pack;
	pack = { rHeader, publicKeyPL };
	send(*sock, (char*)&pack, sizeof(pack), 0);
	sendAESkeyOrReconnectResponse(sock, u);
}



void registrationResponse(SOCKET* sock, User* u)
{
	RegistrationResponse respone;
	recv(*sock, (char*)&respone, sizeof(respone), 0);
	if (respone.header.code != SuccussRegisterResponseCode)
	{
		cout << "Error, receive wrong code" << endl;
		return;
	}
	else
	{
		Encryption::createMeFile(u->name, respone.payload.clientID, Encryption::printPrivateKey(u->priKey));
		strcpy(u->clientID, respone.payload.clientID);
		sendPublicKeyRequest(sock, u);
	}
}


void RegistrationRequest(SOCKET* sock, User* u)
{
	Registration registration;
	RequestHeader rHeader;
	RegistrationRequestPayload rPlayload;
	rHeader.code = RegistrationRequestCode;
	u->name.copy(rPlayload.name, sizeof rPlayload.name);
	rHeader.payload_size = sizeof(rPlayload);
	registration = { rHeader, rPlayload };
	send(*sock, (char*)&registration, sizeof(registration), 0);
	registrationResponse(sock, u);

}
void ReconncetRequest(SOCKET* sock, User* u)
{
	Reconnect reconnect;
	RequestHeader rHeader;
	ReconncetRequestPayload rPlayload;
	rHeader.code = ReconncetRequestCode;
	strcpy(rPlayload.name, u->name.c_str());
	rHeader.payload_size = sizeof(rPlayload);
	reconnect = { rHeader , rPlayload };
	send(*sock, (char*)&reconnect, sizeof(reconnect), 0);
	sendAESkeyOrReconnectResponse(sock , u);
}
int requestHandler(SOCKET *sock, User *u) 
{
	int flag;

	if (u->isNewUser())
	{
		cout << "\nHello new client " << u->name << endl;
		cout << "\nTrying to register you and send your file " << u->FileName << " to our secure server \n" << endl;
		Encryption::EncryptionNewUser(u);
		u->priKey = *new RSA::PrivateKey(u->params);
		u->pubKey = *new RSA::PublicKey(u->params);
		RegistrationRequest(sock,u);
	}
	else
	{
		cout << "\nHello again " << u->name <<  endl;
		cout << "\nTrying to send your file: " << u->FileName << " to our secure server \n" << endl;
		Encryption::EncryptionPublicKeyOldUser(u);
		u->priKey = *new RSA::PrivateKey(u->params);
		u->pubKey = *new RSA::PublicKey(u->params);
		ReconncetRequest(sock, u);
	}
	return 1;
}
