#pragma once

#include "stdafx.h"

enum class StartPoint
{
    BEG,
    CUR
};

class InBinFile
{
public:
    explicit InBinFile(std::string const &file_path);  
    InBinFile(InBinFile const &) = delete;
    InBinFile &operator=(InBinFile const &) = delete;
    
    template <class T>
    size_t Read(T &data);

    template <class U>
    size_t Read(std::vector<U> &data);
    
    bool Seek(size_t offset, StartPoint start = StartPoint::BEG);
    size_t Tell();
    size_t Size() const { return size; }

    ~InBinFile() noexcept;
private:
    std::ifstream file;
    size_t size;
};

/*
Возвращает количество успешно прочитанных байт.
Подходит для 
    - целочисленных типов [int],
    - структур int,
    - статических масивов int.
*/
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

template <class U>
size_t InBinFile::Read(std::vector<U> &data)
{
    try
    {
        file.read(reinterpret_cast<char *>(&data[0]), data.size() * sizeof(U));
    }
    catch(std::exception const &e)
    {
        std::cout << "\n\nRead(T &) throws exception" << std::endl;
        std::cout << e.what() << std::endl;
        file.clear();
    }
    return file.gcount();
}