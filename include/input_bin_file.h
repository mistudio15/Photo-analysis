#pragma once

#include "stdafx.h"

using byte = uint8_t;

enum class StartPoint
{
    BEG,
    CUR
};

enum class Endian
{
    BIG,
    LITTLE
};

class InBinFile
{
public:
    explicit InBinFile(std::string const &file_path) : InBinFile(std::filesystem::path{file_path}) {}
    explicit InBinFile(std::filesystem::path const &file_path);

    InBinFile(InBinFile && other) : file(std::move(other.file)), size(other.size) {};
    InBinFile(InBinFile const &) = delete;
    InBinFile &operator=(InBinFile const &) = delete;

    template <class T>
    size_t Read(T &data);

    virtual size_t Read(std::vector<byte> &data);

    bool Seek(size_t offset, StartPoint start = StartPoint::BEG);
    size_t Tell();
    size_t Size() const { return size; }

    std::string GetFileName() const { return file_path.filename().string(); }

    virtual ~InBinFile() noexcept;
private:
    std::ifstream file;
    std::filesystem::path file_path;
    size_t size;
};

// Паттерн "Декоратор"
// Идея с move-конструктором спасла Вселенную! (UPD: изобрел велосипед...)
class EndianFile : public InBinFile
{
public:
    EndianFile(std::string const &file_path, Endian endian = Endian::BIG) : InBinFile(file_path), endian(endian) {}
    EndianFile(InBinFile &&binFile, Endian endian = Endian::BIG) :
    InBinFile(std::move(binFile)), endian(endian) {}
    void SetEndian(Endian endian_) { endian = endian_; }
    virtual size_t Read(std::vector<byte> &data) override;
private:
    Endian endian;
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
