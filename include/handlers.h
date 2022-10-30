#pragma once

#include "stdafx.h"
#include "input_bin_file.h"

// Паттерн "Централизованная цепочка обязанностей"

class TypeHandler
{
public:
    virtual bool ShouldHandle(int typeDataFormat)  = 0;
    virtual std::string Handle(InBinFile &file, size_t offset)  = 0;
};

class Type3Handler : public TypeHandler
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

class Type5Handler : public TypeHandler
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

class Type2Handler : public TypeHandler
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

class ManagerHandlerType
{
public:
    ManagerHandlerType()
    {
        vecHandlers.push_back(std::make_unique<Type2Handler>());
        vecHandlers.push_back(std::make_unique<Type3Handler>());
        vecHandlers.push_back(std::make_unique<Type5Handler>());
    }
    void Handle(int typeDataFormat, std::string &rawProperty, InBinFile &file, size_t offset);
private:
    std::vector<std::unique_ptr<TypeHandler>> vecHandlers;
};

// -----------------------------------------------------------------------------------


class PropertyHandler
{
public:
    virtual bool ShouldHandle(uint16_t tag) = 0;
    virtual void Handle(std::string &property) = 0;
protected:
    std::string toStringWithoutZeros(double value)
    {
        std::ostringstream stream;
        stream /*<< std::setprecision(1)*/ << std::noshowpoint << value;
        return stream.str();
    }
};

class ExposureTimeHandler : public PropertyHandler
{
public:
    virtual bool ShouldHandle(uint16_t tag) override
    {
        return tag == 0x829a;
    }
    virtual void Handle(std::string &property) override; 
};

class ApertureValueHandler : public PropertyHandler
{
public:
    virtual bool ShouldHandle(uint16_t tag) override
    {
        // return tag == 0x9202;
        return tag == 0x829d;
    }
    virtual void Handle(std::string &property) override; 
};

class FocalLengthHandler : public PropertyHandler
{
public:
    virtual bool ShouldHandle(uint16_t tag) override
    {
        return tag == 0x920a;
    }
    virtual void Handle(std::string &property) override; 
};

class LatitudeLongtitudeRefHandler : public PropertyHandler
{
public:
    virtual bool ShouldHandle(uint16_t tag) override
    {
        return tag == 0x0001 || tag == 0x0003;
    }
    virtual void Handle(std::string &property) override; 
};

class LatitudeLongtitudeHandler : public PropertyHandler
{
public:
    virtual bool ShouldHandle(uint16_t tag) override
    {
        return tag == 0x0002 || tag == 0x0004;
    }
    virtual void Handle(std::string &property) override; 
};

class ManagerHandlerProperty
{
public:
    ManagerHandlerProperty()
    {
        vecHandlers.push_back(std::make_unique<ExposureTimeHandler>());
        vecHandlers.push_back(std::make_unique<ApertureValueHandler>());
        vecHandlers.push_back(std::make_unique<FocalLengthHandler>());
        vecHandlers.push_back(std::make_unique<LatitudeLongtitudeRefHandler>());
        vecHandlers.push_back(std::make_unique<LatitudeLongtitudeHandler>());
    }
    void Handle(uint16_t tag, std::string &property);
private:
    std::vector<std::unique_ptr<PropertyHandler>> vecHandlers;
};