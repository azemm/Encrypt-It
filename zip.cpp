#include "zip.h"

int copy_data(struct archive *ar, struct archive *aw)
{
    int r;
    const void *buff;
    size_t size;
    off_t offset;

    for (;;) {
        r = archive_read_data_block(ar, &buff, &size, &offset);
        if (r == ARCHIVE_EOF)
            return ARCHIVE_OK;
        if (r < ARCHIVE_OK)
            return r;
        r = archive_write_data_block(aw, buff, size, offset);
        if (r < ARCHIVE_OK) {
            return r;
        }
    }
}

Zip::Zip()
{

}

bool Zip::zip(std::string dirToZip, std::string pathToZip)
{
    boost::filesystem::path dir(dirToZip);
    if(boost::filesystem::exists(dir)){
        boost::filesystem::path pathTo(pathToZip);
        boost::filesystem::path currentDir = boost::filesystem::current_path();
        std::string filename(dir.filename().string() + ".zip");

        if(boost::filesystem::exists(pathTo) && boost::filesystem::is_directory(pathTo)){
            boost::filesystem::current_path(pathTo);
        }
        struct archive *a;
        struct archive *disk;
        struct archive_entry *entry;
        int len;
        int buffSize = 8192;
        char buff[buffSize];
        int fd;

        a = archive_write_new();
        archive_write_add_filter_none(a);
        archive_write_set_format_zip(a);
        archive_write_open_filename(a, filename.c_str());

        disk = archive_read_disk_new();
        int path_offset = dir.parent_path().string().size() + 1;
        int r = archive_read_disk_open(disk, dir.c_str());
        for(;;){
            entry = archive_entry_new();
            r = archive_read_next_header2(disk, entry);
            if (r == ARCHIVE_EOF){
                break;
            }
            if (r != ARCHIVE_OK) {
                return false;
            }

            std::string entryPath(archive_entry_pathname(entry));
            entryPath = entryPath.substr(path_offset);

            archive_read_disk_descend(disk);
            archive_entry_set_pathname(entry, entryPath.c_str());
            r = archive_write_header(a, entry);
            if (r < ARCHIVE_OK || r == ARCHIVE_FATAL) {
                return false;
            }
            if (r > ARCHIVE_FAILED) {
                fd = open(archive_entry_sourcepath(entry), O_RDONLY);
                len = read(fd, buff, buffSize);
                while (len > 0) {
                    archive_write_data(a, buff, len);
                    len = read(fd, buff, buffSize);
                }
                close(fd);
            }
            archive_entry_free(entry);
        }
        archive_read_close(disk);
        archive_read_free(disk);
        archive_write_close(a);
        archive_write_free(a);
        boost::filesystem::current_path(currentDir);
        return true;
    }
    return false;
}

bool Zip::unzip(std::string zipFile, std::string pathToExtract)
{
    boost::filesystem::path zip(zipFile);
    if(boost::filesystem::exists(zip)){
        boost::filesystem::path path(pathToExtract);
        boost::filesystem::path currentDir = boost::filesystem::current_path();
        struct archive *a;
        struct archive *ext;
        struct archive_entry *entry;
        int flags =  ARCHIVE_EXTRACT_TIME;
        int r;

        a = archive_read_new();
        archive_read_support_format_all(a);
        archive_read_support_compression_all(a);
        ext = archive_write_disk_new();
        archive_write_disk_set_options(ext, flags);
        archive_write_disk_set_standard_lookup(ext);
        boost::filesystem::current_path(zip.parent_path());
        if ((r = archive_read_open_filename(a, zip.filename().c_str(), 10240)))
            return false;
        for (;;) {
            r = archive_read_next_header(a, &entry);
            if (r == ARCHIVE_EOF)
                break;
            if (r < ARCHIVE_OK || r < ARCHIVE_WARN)
                return false;
            if(boost::filesystem::exists(path) && boost::filesystem::is_directory(path)){
                std::string newPath = path.string() + "/" + std::string(archive_entry_pathname(entry));
                archive_entry_set_pathname(entry, newPath.c_str());
            }
            r = archive_write_header(ext, entry);
            if (r < ARCHIVE_OK)
                return false;
            else if (archive_entry_size(entry) > 0) {
                r = copy_data(a, ext);
                if (r < ARCHIVE_OK || r < ARCHIVE_WARN)
                    return false;
            }
            r = archive_write_finish_entry(ext);
            if (r < ARCHIVE_OK || r < ARCHIVE_WARN)
                return false;
        }
        archive_read_close(a);
        archive_read_free(a);
        archive_write_close(ext);
        archive_write_free(ext);
        boost::filesystem::current_path(currentDir);
        return true;
    }
    return false;
}

