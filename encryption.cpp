#include "encryption.h"

using namespace Botan;

AESEncryption::AESEncryption()
{
    Botan::LibraryInitializer init;
}

AESEncryption::~AESEncryption()
{
}

//salt is stored in the first 16 bytes of the future encrypted file
bool AESEncryption::encrypt(std::string input, std::string output, std::string passphrase)
{
    DataSource_Stream in(input, true);
    DataSink_Stream *out = new DataSink_Stream(output, true);

    AutoSeeded_RNG rng;
    SecureVector<byte> salt = rng.random_vec(16);
    out->write(&salt[0], salt.size());

    return transform(in, out, passphrase, salt, ENCRYPTION);
}

//salt is retrieved by reading the first 16 bytes of the encrypted file
bool AESEncryption::decrypt(std::string input, std::string output, std::string passphrase)
{
    DataSource_Stream in(input, true);
    DataSink_Stream *out = new DataSink_Stream(output, true);

    SecureVector<byte> salt(16);
    in.read(&salt[0], salt.size());
    try{
        return transform(in, out, passphrase, salt,  DECRYPTION);
    } catch(...){
        boost::filesystem::path rm(output);
        boost::filesystem::remove(rm);
        return false;
    }
}

bool AESEncryption::transform(DataSource_Stream &in, DataSink_Stream *out, const std::string &passphrase,const Botan::SecureVector<byte> &salt, Cipher_Dir encryptionMode)
{
    const size_t KEY_LEN = 32;
    const size_t IV_LEN = 16;
    const size_t AES_LEN = KEY_LEN + IV_LEN;

    PBKDF* pbkdf = get_pbkdf("PBKDF2(SHA-256)");

    OctetString aes256_key = pbkdf->derive_key(AES_LEN, passphrase, &salt[0], salt.size(), 10000);
    const byte* mk = aes256_key.begin();

    SymmetricKey key(mk, KEY_LEN);
    InitializationVector iv(&mk[KEY_LEN], IV_LEN);

    Pipe pipe(get_cipher("AES-256/CBC", key, iv,encryptionMode), out);
    pipe.process_msg(in);

    return true;
}

