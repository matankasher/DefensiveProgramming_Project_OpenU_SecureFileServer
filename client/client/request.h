#pragma once
#include <string>
#include <cstdint>
#ifndef MESSAGEU_REQUSET_T_H
#define MESSAGEU_REQUSET_T_H
#define DEFAULT_CLIENT_VERSION 3
using namespace std;
enum RequestCode {
    RegistrationRequestCode = 1100,
    SendpublicKeyRequestCode = 1101,
    ReconncetRequestCode = 1102,
    SendFileRequestCode = 1103,
    ValidCRCRequestCode = 1104,
    InvalidCRCRequestCode = 1105,
    FourthInvalidCRCRequestCode = 1106   
};

#pragma pack(push, 1)
struct RequestHeader {
    char client_id[16] = {};
    uint8_t version = DEFAULT_CLIENT_VERSION;
    uint16_t code{};
    uint32_t payload_size{};
};
#pragma pack(pop)

/********************
 * Payloads structs - for each kind of request we'll need to support different payload structure
 ********************/

 // 1100: Registration request:
#pragma pack(push, 1)
struct RegistrationRequestPayload {
    char name[255] = {};
};
#pragma pack(pop)

// 1101: Send Public key request:
#pragma pack(push, 1)
struct SendpublicKeyPayload
{
    char name[255] = {};
    char publicKey[1024] = {};
};
#pragma pack(pop)

// 1102:  Reconncet request:
#pragma pack(push, 1)
struct ReconncetRequestPayload {
    char name[255] = {};
};
#pragma pack(pop)

// 1103: Send file request:
#pragma pack(push, 1)
struct SendFileRequestPayload
{
    uint32_t content_size{};
    char fileName[255];

};
#pragma pack(pop)

// 1104: validCRC:
#pragma pack(push, 1)
struct ValidCRCPayload 
{
    char fileName[255];
};
#pragma pack(pop)
// 1105: InvalidCRC:
#pragma pack(push, 1)
struct InvalidCRCPayload
{
    char fileName[255];
};
#pragma pack(pop)

// 1106: Fourth time invalid CRC done:
#pragma pack(push, 1)
struct FourthInvalidCRCPayload
{
    char fileName[255];
};
#pragma pack(pop)

/********************
 * Request struct - packing header and payload type together
 ********************/

#pragma pack(push, 1)
struct Registration {
    RequestHeader header;
    RegistrationRequestPayload payload;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SendpublicKey {
    RequestHeader header;
    SendpublicKeyPayload payload;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Reconnect {
    RequestHeader header;
    ReconncetRequestPayload payload;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SendFile {
    RequestHeader header;
    SendFileRequestPayload payload;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct ValidCRC {
    RequestHeader header;
    ValidCRCPayload payload;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct InvalidCRC {
    RequestHeader header;
    InvalidCRCPayload payload;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct FourthInvalidCRC {
    RequestHeader header;
    FourthInvalidCRCPayload payload;
};
#pragma pack(pop)



#endif //MESSAGEU_REQUSET_T_H

