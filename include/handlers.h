#pragma once

#include "stdafx.h"
#include "input_bin_file.h"

// Паттерн "Централизованная цепочка обязанностей"
class Handler
{
public:
    virtual bool ShouldHandle(int typeDataFormat)  = 0;
    virtual std::string Handle(InBinFile &file, size_t offset)  = 0;
};

class Type3Handler : public Handler
{
public:
    // Type3Handler(size_t offset) : Handler(offset) {}
    bool ShouldHandle(int typeDataFormat)  override
    {
        return typeDataFormat == 3;
    }
    std::string Handle(InBinFile &file, size_t offset)  override;
private:
    size_t sizeComponent = 2;
};

class Type5Handler : public Handler
{
public:
    // Type5Handler(size_t offset) : Handler(offset) {}
    bool ShouldHandle(int typeDataFormat)  override
    {
        return typeDataFormat == 5;
    }
    std::string Handle(InBinFile &file, size_t offset)  override;
private:
    size_t sizeComponent = 8;
};

class Type2Handler : public Handler
{
public:
    // Type2Handler(size_t offset) : Handler(offset) {}
    bool ShouldHandle(int typeDataFormat)  override
    {
        return typeDataFormat == 2;
    }
    std::string Handle(InBinFile &file, size_t offset)  override;
private:
    size_t sizeComponent = 1;
};
