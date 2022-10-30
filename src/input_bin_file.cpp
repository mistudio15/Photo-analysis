#include "input_bin_file.h"
#include "extract_exif.h"

using std::cout;
using std::endl;

InBinFile::InBinFile(std::filesystem::path const &file_path) : file(file_path.string(), std::ios::binary), file_path(file_path)
{
    if (!file.is_open())
    {
        throw std::runtime_error("file \"" + file_path.string() + "\" is not open");
    }
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    // определяем размер файла в байтах
    file.seekg(0, std::ios_base::end);
    size = file.tellg();
    file.seekg(0, std::ios_base::beg);
}

size_t InBinFile::Read(std::vector<byte> &data)
{
    try
    {
        file.read(reinterpret_cast<char *>(&data[0]), data.size());
    }
    catch(std::exception const &e)
    {
        std::cout << "\n\nRead(T &) throws exception" << std::endl;
        std::cout << e.what() << std::endl;
        file.clear();
    }
    return file.gcount();
}

size_t EndianFile::Read(std::vector<byte> &data)
{
    size_t nBytes = InBinFile::Read(data);
    if (endian == Endian::LITTLE)
    {
        ReverseBytes(data);
    }
    return nBytes;
}

bool InBinFile::Seek(size_t offset, StartPoint start)
{
    if (start == StartPoint::BEG)
    {
        if (offset > size)
        {
            return false;
        }
        file.seekg(offset, std::ios_base::beg);
    }
    else if (start == StartPoint::CUR)
    {
        if (offset + file.tellg() > size)
        {
            return false;
        }
        file.seekg(offset, std::ios_base::cur);
    }
    return true;
}

size_t InBinFile::Tell()
{
    return file.tellg();
}

InBinFile::~InBinFile()
{
    if (file.is_open())
    {
        file.close();
    }
}
