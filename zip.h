#ifndef ZIP_H
#define ZIP_H
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <archive.h>
#include <archive_entry.h>
#include <boost/filesystem.hpp>

class Zip
{
public:
    Zip();
    static bool zip(std::string dirToZip, std::string pathToZip = "");
    static bool unzip(std::string zipFile, std::string pathToExtract = "");
};

#endif // ZIP_H
