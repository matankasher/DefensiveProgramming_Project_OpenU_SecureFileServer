import os
import struct
import uuid
from Crypto.Cipher import PKCS1_OAEP
from Crypto.PublicKey import RSA

from AES import create_aes,  Decrypt_Content, caculate_crc
from db_sqlite import is_old_client, insert_client, insert_file
from enumFields import Generals, RequestCode, ResponseCode, print_respone_code, print_request_code


class Response:   # respone packs to send
    def __init__(self, version, code, payload=None):
        if payload is None:
            payload = {}
        self.version = version
        self.code = code
        self.payload_size = int()
        self.payload = payload

    def pack(self):
        packed_payload = self.pack_payload()
        self.payload_size = int(len(packed_payload))
        return self.pack_header() + packed_payload  # return header and payload.

    def pack_header(self):
        return struct.pack("<B H I", self.version, self.code, self.payload_size) # header struct 

    def pack_payload(self): # packing according the code value
        pack_format = str()
        values_to_pack = tuple()
        if self.code == 2100 or self.code == 2106:
            pack_format = "<16s"
            values_to_pack = (bytes(self.payload, encoding=Generals.UTF8.value),)
        if self.code == 2102 or self.code == 2105:
            pack_format = "<16s " + str(len(self.payload["encrypted_aes_key"])) +"s" # caculate size of encrypted aes
            values_to_pack = (
                bytes(self.payload["client_id"], encoding=Generals.UTF8.value),
                self.payload["encrypted_aes_key"])
        if self.code == 2103:
            pack_format = "<16s 4s 255s 8s"
            values_to_pack = (
                bytes(self.payload["client_id"], encoding=Generals.UTF8.value),
                bytes(self.payload["content_size"], encoding=Generals.UTF8.value),
                bytes(self.payload["file_name"], encoding=Generals.UTF8.value),
                bytes(self.payload["cksum"], encoding=Generals.UTF8.value))
        if self.code == 2104:
            pack_format = "<16s"
            values_to_pack = (bytes(self.payload, encoding=Generals.UTF8.value),)
        return struct.pack(pack_format, *values_to_pack)


def generate_client_id():
    unique_id = uuid.uuid4().hex[:16]
    return unique_id


def receive_request_header(sock):
    data = sock.recv(23)  # Size of struct is 23 bytes
    try:
        client_id, version, code, payload_size = struct.unpack('<16sBHI', data)
        client_id = client_id.strip(b'\x00').decode()
        print(print_request_code(code))
        return client_id, version, code, payload_size
    except:
        print("error at request header")
        return


def receive_registration(sock, payload_size):
    data = sock.recv(payload_size)
    name = data.strip(b'\x00').decode()
    return name


def receive_publickey(sock, payload_size):
    data = sock.recv(payload_size)
    name = data[:255]
    publicKey = data[160:]
    name = name.strip(b'\x00').decode()
    publicKey = publicKey.strip(b'\x00').decode()
    return publicKey.strip()

def send_respone(sock, version, code, payload):

    print(print_respone_code(code))
    res = Response(version, code, payload)
    sock.sendall(res.pack())

def receive_file_payload(sock, payload_size):
    data = sock.recv(259)
    content_size, file_name = struct.unpack('<I 255s', data)
    file_name = file_name.strip(b'\x00').decode()
    messageContent = sock.recv(content_size)
    return file_name, messageContent

def save_file(connection,file_name,decrypt_content, client_id):
    f = open(file_name, "w")
    f.write(decrypt_content)
    insert_file(client_id, file_name, os.path.abspath(file_name), "true")
    send_respone(connection, Generals.version.value, ResponseCode.ConfirmReceivedMessageResponseCode.value,client_id)


def falidCRC(client_id, file_name):
    print("Failed to verify CRC ")
    insert_file(client_id, file_name, os.path.abspath(file_name), "false")  # save in DB
    return


def file_handler(connection, AES_key):
    client_id, version, code, payload_size = receive_request_header(connection)
    if code != RequestCode.SendFileRequestCode.value:
        send_respone(connection, Generals.version.value, ResponseCode.GeneralErrorResponseCode.value) # general error respone
        print("code error!")
        return
    file_name, messageContent = receive_file_payload(connection, payload_size)
    decrypt_content = Decrypt_Content(AES_key, messageContent) # try to decrypt the file content with the AES key
    send_respone(connection, Generals.version.value, ResponseCode.ReceivedValidFileWithCRCResponseCode.value,
                 {"client_id": client_id, "content_size": str(len(messageContent)) , "file_name": file_name, "cksum": caculate_crc(decrypt_content)[-8:]})
    while 1 == 1: # crc handler
        client_id, version, code, payload_size = receive_request_header(connection)
        if code == RequestCode.FourthInvalidCRCRequestCode.value:  # four time failed to verify CRC
            falidCRC(client_id, file_name)
            return
        elif code == RequestCode.ValidCRCRequestCode.value:      # success to verify CRC
            save_file(connection, file_name,decrypt_content, client_id)
            return
        else:  # Another try to verify CRC
            send_respone(connection, Generals.version.value, ResponseCode.ReceivedValidFileWithCRCResponseCode.value,
                         {"client_id": client_id, "content_size": str(len(messageContent)), "file_name": file_name,
                          "cksum": caculate_crc(decrypt_content)[-8:]})

def AES_request_handler(connection, client_id, public_key_string, code, name):
    public_key = RSA.import_key(public_key_string)
    cipher = PKCS1_OAEP.new(public_key)
    AES_key = create_aes()
    insert_client(client_id, name, public_key_string, AES_key)  # insert to database
    enc_AES = cipher.encrypt(AES_key.encode('utf-8'))  # encrypt AES key with public key
    send_respone(connection, Generals.version.value, code , {"client_id": client_id, "encrypted_aes_key": enc_AES})
    file_handler(connection,AES_key)

def first_connect_request_handler(connection):
    publicKey = ""
    next_code = ResponseCode.SendAESkeyResponseCode.value
    client_id, version, code, payload_size = receive_request_header(connection)
    if code != RequestCode.RegistrationRequestCode.value and code != RequestCode.ReconncetRequestCode.value:
        send_respone(connection, Generals.version.value, ResponseCode.GeneralErrorResponseCode.value) # general error respone
        print("code error!")
        return
    name = receive_registration(connection, payload_size)
    if code == RequestCode.RegistrationRequestCode.value:  # new user
        client_id = generate_client_id()
        send_respone(connection, Generals.version.value, ResponseCode.SuccessRegisterResponseCode.value, client_id)
        client_id, version, code, payload_size = receive_request_header(connection)
        publicKey = receive_publickey(connection, payload_size)
    else:  # old user
        publicKey = is_old_client(name)
        if publicKey == "null":  # old user ,not found the user in the DB
            send_respone(connection, Generals.version.value, ResponseCode.RequestToReconncetDeniedResponseCode.value, client_id)
            client_id, version, code, payload_size = receive_request_header(connection)
            if code != RequestCode.RegistrationRequestCode.value:
                send_respone(connection, Generals.version.value,
                             ResponseCode.GeneralErrorResponseCode.value,0)  # general error respone
                print("code error!")
                return
            # re-registration
            name = receive_registration(connection, payload_size)
            client_id = generate_client_id()
            send_respone(connection, version, ResponseCode.SuccessRegisterResponseCode.value, client_id)
            client_id, version, code, payload_size = receive_request_header(connection)
            publicKey = receive_publickey(connection, payload_size)
        else:   # old user , found the user in the DB
            publicKey = publicKey[0]
            next_code = ResponseCode.ConfirmRequestToReconncetResponseCode.value
    AES_request_handler(connection, client_id, publicKey, next_code, name)
