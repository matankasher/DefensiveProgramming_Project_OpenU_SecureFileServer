#pragma once
#include <string>
#include <cstdint>
#ifndef MESSAGEU_RESPONE_T_H
#define MESSAGEU_RESPONE_T_H
#define DEFAULT_CLIENT_VERSION 3
enum ResponseCode {
	SuccussRegisterResponseCode = 2100,
	FailRegisterResponseCode = 2101,
	SendAESkeyResponseCode = 2102,
	ReceivedValidFileWithCRCResponseCode = 2103,
	ConfirmReceivedMessageResponseCode = 2104,
	ConfirmRequestToReconncetResponseCode = 2105,
	RequestToReconncetDeniedResponseCode = 2106,
	GeneralErrorResponseCode = 2107
};
#pragma pack(push, 1)
struct ResponeHeader {
	uint8_t version = DEFAULT_CLIENT_VERSION;
	uint16_t code{};
	uint32_t payload_size{};
};
#pragma pack(pop)
/********************
 * Payloads structs - for each kind of respone we'll need to support different payload structure
 ********************/

 // 2100: Succuss Register Response :
#pragma pack(push, 1)
struct SuccussRegisterResponsePayload {
	char clientID[16] = {};
};
#pragma pack(pop)

// 2101: Failed Register Response:
#pragma pack(push, 1)
struct FailedRegisterResponsePayload {
};
#pragma pack(pop)


// 2102: Succuss Register Response :
#pragma pack(push, 1)
struct SendAESkeyResponsePayload {
	char clientID[16] = {};
	char encryptedAESkey[128] = {};
};
#pragma pack(pop)

// 2103: Recieve Valid File With CRC Response :
#pragma pack(push, 1)
struct RecieveValidFileWithCRCResponsePayload {
	char clientID[16] = {};
	char contentSize[4] = {};
	char fileName[255] = {};
	char cksum[8] = {};
};
#pragma pack(pop)

// 2104: comfirm recieve message :
#pragma pack(push, 1)
struct ComfirmRecieveMessageResponsePayload {
	char clientID[16] = {};
};
#pragma pack(pop)
// 2105: confirm request to reconncet
#pragma pack(push, 1)
struct ComfirmRequestReconncetResponsePayload {
	char clientID[16] = {};
	char encryptedAESkey[128] = {};
};

// 2106: Rejected to reconnect , do new Register
#pragma pack(push, 1)
struct NewRegisterResponsePayload {
	char clientID[16] = {};
};
// 2107: general error
#pragma pack(push, 1)
struct GeneralErrorResponsePayload {
};

#pragma pack(push, 1)
struct RegistrationResponse {
	ResponeHeader header;
	SuccussRegisterResponsePayload payload;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct FailedRegisterResponse {
	ResponeHeader header;
	FailedRegisterResponsePayload payload;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SendAESkeyResponse {
	ResponeHeader header;
	SendAESkeyResponsePayload payload;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct RecieveValidFileWithCRCResponse {
	ResponeHeader header;
	RecieveValidFileWithCRCResponsePayload payload;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct ComfirmRecieveMessageResponse {
	ResponeHeader header;
	ComfirmRecieveMessageResponsePayload payload;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct ComfirmRequestReconncetResponse {
	ResponeHeader header;
	ComfirmRequestReconncetResponsePayload payload;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct NewRegisterResponse {
	ResponeHeader header;
	NewRegisterResponsePayload payload;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct GeneralErrorResponse {
	ResponeHeader header;
	GeneralErrorResponsePayload payload;
};
#pragma pack(pop)

#endif //MESSAGEU_RESPONE_T_H