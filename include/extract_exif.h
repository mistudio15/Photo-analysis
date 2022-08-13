#pragma once

#include "stdafx.h"
#include "input_bin_file.h"

using byte      = uint8_t;
using bytes     = std::vector<uint8_t>;

class InBinFile;

// для enum class нужно перегружать operator|

enum Tags : byte
{
    ExposureTime        = 0b000001,
    ISO                 = 0b000010,
    DateTimeOriginal    = 0b000100,
    ApertureValue       = 0b001000,
    FocalLength         = 0b010000
};

union SettedTags
{
    struct
    {
        byte isExposureTime      : 1;
        byte isISO               : 1;
        byte isDateTimeOriginal  : 1;
        byte isApertureValue     : 1;
        byte isFocalLength       : 1;
    };
    byte flags;
};


// Паттерн "Централизованная цепочка обязанностей"
class Handler
{
public:
    Handler(size_t offset) : m_offset(offset) {}
    virtual bool ShouldHandle(int typeDataFormat)  = 0;
    virtual double Handle(InBinFile &file)  = 0;
protected:
    size_t m_offset;
};

class Type3Handler : public Handler
{
public:
    Type3Handler(size_t offset) : Handler(offset) {}
    bool ShouldHandle(int typeDataFormat)  override 
    {
        return typeDataFormat == 3;
    }
    double Handle(InBinFile &file)  override;
};

class Type5Handler : public Handler
{
public:
    Type5Handler(size_t offset) : Handler(offset) {}
    bool ShouldHandle(int typeDataFormat)  override
    {
        return typeDataFormat == 5;
    }
    double Handle(InBinFile &file)  override;
};

bool HasExif(InBinFile &file);

// std::unordered_map<Tags, double> ExtractExif(InBinFile &file, SettedTags settedTags); 
std::unordered_map<uint32_t, double> ExtractExif(InBinFile &file, std::vector<bytes> const &vecTags); 

uint64_t MergeBytes(bytes const &vecBytes);

void ReverseBytes(bytes &bytes);

std::ostream &operator <<(std::ostream &out, Tags const &tag);



