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

bool HasExif(InBinFile &file);

std::unordered_map<Tags, double> ExtractExif(InBinFile &file, SettedTags settedTags); 

uint64_t MergeBytes(bytes const &vecBytes);

void ReverseBytes(bytes &bytes);

std::ostream &operator <<(std::ostream &out, Tags const &tag);