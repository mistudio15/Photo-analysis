#include "input_bin_file.h"

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

// возвращает количество успешно прочитанных байт
template <class T>
size_t InBinFile::Read(T &data)
{
    try
    {
        file.read(reinterpret_cast<char *>(&data), sizeof(T));
    }
    catch(std::exception const &e)
    {
        std::cout << "\n\nRead(T &) throws exception" << std::endl;
        std::cout << e.what() << std::endl;
        file.clear();
    }
    return file.gcount();
}

bool InBinFile::Seek(size_t pos_read)
{
    if (pos_read > size)
    {
        return false;
    }
    file.seekg(pos_read, std::ios_base::beg);
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