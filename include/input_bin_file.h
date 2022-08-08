#pragma once

#include "stdafx.h"

class InBinFile
{
public:
    explicit InBinFile(std::string const &file_path);  
    InBinFile(InBinFile const &) = delete;
    InBinFile &operator=(InBinFile const &) = delete;
    
    template <class T>
    size_t Read(T &data);
    
    bool Seek(size_t pos_read);
    size_t Tell();
    size_t Size() const { return size; }

    ~InBinFile() noexcept;
private:
    std::ifstream file;
    size_t size;
};