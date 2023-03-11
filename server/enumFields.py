from enum import Enum


def print_respone_code(code):
    switcher = {
        ResponseCode.SuccessRegisterResponseCode.value: "success Register user            Response Code: 2100\n",
        ResponseCode.FailRegisterResponseCode.value: "failed to register user          Response Code: 2101\n",
        ResponseCode.SendAESkeyResponseCode.value: "send send AES key                Response Code: 2102\n",
        ResponseCode.ReceivedValidFileWithCRCResponseCode.value: "valid file with CRC              Response Code: 2103\n",
        ResponseCode.ConfirmReceivedMessageResponseCode.value: "Confirm Received Message         Response Code: 2104\n",
        ResponseCode.ConfirmRequestToReconncetResponseCode.value: "success Re-connect               Response Code: 2105\n",
        ResponseCode.RequestToReconncetDeniedResponseCode.value: "request to reconnect denied     Response Code: 2106\n",
        ResponseCode.GeneralErrorResponseCode.value: "general error                    Response Code: 2107\n",
    }
    return switcher.get(code)


def print_request_code(code):
    switcher = {
        RequestCode.RegistrationRequestCode.value: " Register user                   Request Code: 1100\n",
        RequestCode.SendpublicKeyRequestCode.value: "Send public Key                  Request Code: 1101\n",
        RequestCode.ReconncetRequestCode.value: "Reconncet Request                Request Code: 1102\n",
        RequestCode.SendFileRequestCode.value: "Send File                        Request Code: 1103\n",
        RequestCode.ValidCRCRequestCode.value: "Valid CRC                        Request Code: 1104\n",
        RequestCode.InvalidCRCRequestCode.value: "Invalid CRC                      Request Code: 1105\n",
        RequestCode.FourthInvalidCRCRequestCode.value: "Fourth  Invalid  CRC             Request Code: 1106\n",
    }
    return switcher.get(code)


class Generals(Enum):
    structBytes = 23
    UTF8 = "UTF-8"
    DBServer = 'server.db'
    create_client_table = """CREATE TABLE  clients(
                 ID CHAR(16),
                 NAME TEXT NOT NULL,
                 PUBLICKEY CHAR(160) NOT NULL,
                 LASTSEEN TEXT NOT NULL,
                 AESKEY CHAR(16)   
                 );"""
    create_file_table = """CREATE TABLE  files(
                 ID CHAR(16),
                 FILENAME CHAR(255) NOT NULL,
                 PATHNAME CHAR(255) NOT NULL,
                 VERIFIED TEXT NOT NULL   
                 );"""
    version = 3


class RequestCode(Enum):
    RegistrationRequestCode = 1100
    SendpublicKeyRequestCode = 1101
    ReconncetRequestCode = 1102
    SendFileRequestCode = 1103
    ValidCRCRequestCode = 1104
    InvalidCRCRequestCode = 1105
    FourthInvalidCRCRequestCode = 1106


class ResponseCode(Enum):
    SuccessRegisterResponseCode = 2100
    FailRegisterResponseCode = 2101
    SendAESkeyResponseCode = 2102
    ReceivedValidFileWithCRCResponseCode = 2103
    ConfirmReceivedMessageResponseCode = 2104
    ConfirmRequestToReconncetResponseCode = 2105
    RequestToReconncetDeniedResponseCode = 2106
    GeneralErrorResponseCode = 2107
