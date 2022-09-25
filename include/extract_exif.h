#pragma once

#include "stdafx.h"
#include "input_bin_file.h"
#include "handlers.h"

using byte      = uint8_t;
using bytes     = std::vector<uint8_t>;

enum Tags : uint32_t
{
    ExposureTime        = 0x829a,
    ISO                 = 0x8827,
    DateTimeOriginal    = 0x9003,
    ApertureValue       = 0x9202,
    FocalLength         = 0x920A
};

struct ReportExtraction
{
    bool done = false;
    std::string file_name;
    std::vector<uint16_t> notFoundTags;
    std::unordered_map<uint16_t, std::string> mapData;
    void Clear()
    {
        done = false;
        file_name.clear();
        notFoundTags.clear();
        mapData.clear();
    }
};

uint64_t MergeBytes(bytes const &vecBytes);

void ReverseBytes(bytes &bytes);

std::ostream &operator <<(std::ostream &out, Tags const &tag);

class ExtracterExif
{
public:
    ExtracterExif(std::vector<uint16_t> const &tags = {});
    void AddRef(uint16_t ref) { vecRefs.push_back(ref); }
    void AddTag(uint16_t tag) { vecTags.push_back(tag); }
    void AddTags(std::vector<uint16_t> const &tags)
    {
        std::copy(tags.begin(), tags.end(), back_inserter(vecTags));
    }
    void AddRefs(std::vector<uint16_t> const &refs)
    {
        std::copy(refs.begin(), refs.end(), back_inserter(vecRefs));
    }

    void ClearTags() { vecTags.clear(); }
    ReportExtraction ExtractExif(InBinFile &file);
    inline static const std::unordered_map<uint16_t, std::string> mapTagsName = {
        { 0x829a, "ExposureTime" },
        { 0x8827, "ISO" },
        { 0x9003, "DateTimeOriginal" },
        { 0x829d, "F-Number" },
        { 0x920a, "FocalLength" }
    };
private:
    size_t offset;
    std::vector<std::unique_ptr<Handler>> vecHandlers;
    std::vector<uint16_t> vecTags;
    std::vector<uint16_t> vecRefs = { 0x8769, 0x8825 };
    ReportExtraction report;
    bytes buf4 = bytes(4);
    bytes buf2 = bytes(2);
    
    void Parse(InBinFile &file);
    bool HasExif(InBinFile &file);
};




