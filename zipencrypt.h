#ifndef ZIPENCRYPT_H
#define ZIPENCRYPT_H

#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include "zip.h"
#include "encryption.h"

class ZipEncrypt
{
public:
    ZipEncrypt();
    bool zipEncrypt(std::string dir, std::string password, bool delTemps = false, bool original = false);
    bool zipDecrypt(std::string dir, std::string password, bool delTemps = false, bool original = false);

private:
    AESEncryption encryption;

};

#endif // ZIPENCRYPT_H
