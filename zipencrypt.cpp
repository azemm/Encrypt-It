#include "zipencrypt.h"

ZipEncrypt::ZipEncrypt() : encryptedExtension(".ec")
{

}

bool ZipEncrypt::zipEncrypt(std::string dir, std::string password, bool delTemps, bool original)
{
    bool result = false;
    boost::filesystem::path dirPath(dir);
    bool zipSucess = Zip::zip(dir, dirPath.parent_path().string());
    if(zipSucess){
        std::string zippedDir = dir + ".zip";
        std::string encryptedZippedDir = zippedDir + encryptedExtension;
        result = encryption.encrypt(zippedDir, encryptedZippedDir, password);

        if(result && delTemps){
            boost::filesystem::path dirZip(zippedDir);
            boost::filesystem::remove(dirZip);
        }

        if(result && original){
            boost::filesystem::remove_all(dirPath);
        }

        return result;
    }

    return false;
}

bool ZipEncrypt::zipDecrypt(std::string file, std::string password, bool delTemps, bool original)
{
    bool result = false;
    boost::filesystem::path encryptedFile(file);
    if(boost::filesystem::exists(encryptedFile)){
        std::string zippedDir = boost::algorithm::replace_last_copy(file, encryptedExtension, "");
        bool success =  encryption.decrypt(file, zippedDir, password);
        boost::filesystem::path zipPath(zippedDir);
        if(success && boost::filesystem::exists(zipPath)){
            result = Zip::unzip(zippedDir, zipPath.parent_path().string());
        }

        if(result && delTemps){
            boost::filesystem::path dirZip(zippedDir);
            boost::filesystem::remove(dirZip);
        }

        if(result && original){
            boost::filesystem::remove(encryptedFile);
        }

        return result;
    }

    return false;
}

void ZipEncrypt::setEncryptedExtension(std::string ext)
{
    this->encryptedExtension = ext;
}

