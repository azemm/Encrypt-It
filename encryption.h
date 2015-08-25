#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include "zip.h"
#include <botan/botan.h>
#include <botan/cryptobox.h>
#include <iostream>
#include <fstream>

class AESEncryption
{
public:
    AESEncryption();
    virtual ~AESEncryption();

    bool encrypt(std::string input, std::string output, std::string passphrase);
    bool decrypt(std::string input, std::string output, std::string passphrase);

private:
    bool transform(Botan::DataSource_Stream &in, Botan::DataSink_Stream *out, const std::string &passphrase,const Botan::SecureVector<Botan::byte> &salt, Botan::Cipher_Dir encryptionMode);
};

#endif // ENCRYPTION_H
