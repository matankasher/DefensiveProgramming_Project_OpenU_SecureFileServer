#include "cryptlib.h"
#include "rijndael.h"
#include "modes.h"
#include "files.h"
#include "osrng.h"
#include "hex.h"
#include "pem.h"
#include <iostream>
#include <string>
#include <cstring>
#include <rsa.h>
#include "User.h"
#include "base64.h"

using namespace CryptoPP;
using namespace std;
class Encryption
{
public:
    static void EncryptionNewUser(User* u);
    static void EncryptionPublicKeyOldUser(User* u);
    static string printPrivateKey(RSA::PrivateKey key);
    static string printPublicKey(RSA::PublicKey key);
    static void createMeFile(string name, string clientID, string priKey);
    static string strip(const std::string& inpt);
    static string encrypt(const string& plain, User* u);
    static string decrypt(const string& cipher, User* u);
    static void AESWrapper(const  char* key, unsigned int length, User* u);
    static string AESencrypt(const char* plain, unsigned int length, User* u);
    static string AESdecrypt(const char* cipher, unsigned int length, User* u);
    static boolean CheckCRC(string fileContent, char serverCRC[]);
};

