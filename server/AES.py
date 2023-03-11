import secrets
from base64 import b64encode, b64decode
from binascii import unhexlify
import crcmod.predefined
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad

def create_aes():
    return secrets.token_hex(8)


def Decrypt_Content(key_str, encrypted_message):
    key = key_str.encode('utf-8')[:16]
    iv = bytes([0]*AES.block_size)
    aes = AES.new(key, AES.MODE_CBC, iv)
    decrypted_bytes = aes.decrypt(encrypted_message)
    # Convert decrypted bytes to string and return
    decrypted_str = decrypted_bytes.decode('utf-8')
    padding_size = ord(decrypted_str[-1])
    unpadded_str = decrypted_str[:-padding_size]
    # Print the decrypted string as base64
    return unpadded_str

def caculate_crc(decrypt_message):
    crc32_func = crcmod.predefined.mkCrcFun('crc-32')
    checksum = crc32_func(decrypt_message.encode())
    return hex(checksum)