#include "Encryption.h"

uint32_t crc32(const uint8_t* data, size_t length) {  // caculate crc 32 value
    const uint32_t polynomial = 0xEDB88320;
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (size_t j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ polynomial;
            }
            else {
                crc >>= 1;
            }
        }
    }
    return crc ^ 0xFFFFFFFF;
}

string uint32_to_hex(uint32_t value)  // convert crc to hex to compare the crc from the server side
{
    stringstream stream;
    stream << hex << value;
    return stream.str();
}
boolean validCRC(string clientCRC, string serverCRC)  // compare between the crc
{
    for (int i = 0; i < 8; i++)
    {
        if (clientCRC[i] != serverCRC[i])
            return false;
    }
    return true;
}

boolean Encryption::CheckCRC(string fileContent, char serverCRC[])   // check if the crc's are equals
{
   string clientCRC  = uint32_to_hex(crc32(reinterpret_cast<const uint8_t*>(fileContent.c_str()), fileContent.length()));
   return validCRC(clientCRC, serverCRC);
}




string PKCS7_pad(const std::string& data, size_t block_size)  // pad to eb
{
    size_t padding_size = block_size - data.size() % block_size;
    char padding_byte = static_cast<char>(padding_size);
    std::string padding(padding_size, padding_byte);
    return data + padding;
}
string Encryption::AESencrypt(const char* plain, unsigned int length, User *u)
{
    CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE] = { 0 };	// for practical use iv should never be a fixed value!

    CryptoPP::AES::Encryption aesEncryption(u->_key, u->DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, iv);
    std::string cipher;
    CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(cipher));
    stfEncryptor.Put(reinterpret_cast<const CryptoPP::byte*>(plain), length);
    stfEncryptor.MessageEnd();
    return cipher;
}

string  Encryption::AESdecrypt(const char* cipher, unsigned int length, User* u)
{
    CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE] = { 0 };	// for practical use iv should never be a fixed value!
    CryptoPP::AES::Decryption aesDecryption(u->_key, u->DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv);
    std::string decrypted;
    CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink(decrypted));
    stfDecryptor.Put(reinterpret_cast<const CryptoPP::byte*>(cipher), length);
    stfDecryptor.MessageEnd();

    return decrypted;
}




string Encryption::encrypt(const string& plain, User* u)  // encrypt with public key
{
    std::string cipher;
    CryptoPP::RSAES_OAEP_SHA_Encryptor e(u->pubKey);
    CryptoPP::StringSource ss(plain, true, new CryptoPP::PK_EncryptorFilter(u->_rng, e, new CryptoPP::StringSink(cipher)));
    return cipher;
}

string Encryption::decrypt(const string& cipher, User* u) // decrypt with private key
{
    std::string decrypted;
    CryptoPP::RSAES_OAEP_SHA_Decryptor d(u->priKey);
    CryptoPP::StringSource ss_cipher(cipher, true, new CryptoPP::PK_DecryptorFilter(u->_rng, d, new CryptoPP::StringSink(decrypted)));
    return decrypted;
}


void Encryption::AESWrapper(const char* key, unsigned int length, User* u) // save the aes key from the server
{
    if (length != u->DEFAULT_KEYLENGTH)
        throw std::length_error("key length must be 16 bytes");
    memcpy_s(u->_key, u->DEFAULT_KEYLENGTH, key, length);
}


string Encryption::strip(const std::string& inpt) // strip spaceChars from the public key in the file
{
    auto start_it = inpt.begin();
    auto end_it = inpt.rbegin();
    while (std::isspace(*start_it))
        ++start_it;
    while (std::isspace(*end_it))
        ++end_it;
    return std::string(start_it, end_it.base());
}
void Encryption::EncryptionNewUser(User *u)
{
    u->params.GenerateRandomWithKeySize(u->_rng , 1024);
}

string Encryption::printPrivateKey(RSA::PrivateKey key)
{
    string pem;
    StringSink sink(pem);
    PEM_Save(sink, key);
    return pem;
}
string Encryption::printPublicKey(RSA::PublicKey key)
{
    string pem;
    StringSink sink(pem);
    PEM_Save(sink, key);
    return pem;
}
void Encryption::createMeFile(string name, string clientID, string priKey)
{
    fstream file;
    file.open("me.info", ios::out);
    if (!file)
    {
        cout << "Error in creating file!!!";
        return;
    }
    file << name << endl;
    file << clientID << endl; // ascii special number
    file << priKey << endl;
    file.close();
}
void Encryption::EncryptionPublicKeyOldUser(User *u)   // read the exist public key
 {
        string Privatekey = "", temp, publickey;
        ifstream ReadKey("me.info");
        getline(ReadKey, temp); // advance the pointer of the read file.
        getline(ReadKey, temp);
        ostringstream sstream;
        sstream << ReadKey.rdbuf(); // reading data
        Privatekey = sstream.str(); 
        Privatekey = strip(Privatekey) + '\n';
        CryptoPP::StringSource PKeyStringSource(Privatekey, true);
        RSA::PrivateKey priKey;
        PEM_Load(PKeyStringSource, priKey);
        u->params = *new InvertibleRSAFunction(priKey);
 }