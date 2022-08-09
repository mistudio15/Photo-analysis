#include "input_bin_file.h"

using std::cout;
using std::endl;

InBinFile::InBinFile(std::string const &file_path) : file(file_path, std::ios::binary)
{
    if (!file.is_open())
    {   
        throw std::runtime_error("file \"" + file_path + "\" is not open");
    }
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    // определяем размер файла в байтах
    file.seekg(0, std::ios_base::end);
    size = file.tellg();
    file.seekg(0, std::ios_base::beg);
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
    file.close();
}