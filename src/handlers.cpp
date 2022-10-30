#include "handlers.h"
#include "extract_exif.h"

class InBinFile;

// uint8_t ASCII
std::string Type2Handler::Handle(InBinFile &file, size_t offset)
{
    bytes buf4(4);
    bytes addres(4);
    file.Read(buf4);
    int nComponents = MergeBytes(buf4);
    std::string result;
    if (nComponents * sizeComponent > 4)
    {
        file.Read(addres);

        size_t save_pos = file.Tell();

        file.Seek(offset + MergeBytes(addres));

        uint8_t symbol;
        for (size_t i = 0; i < nComponents; ++i)
        {
            file.Read(symbol);
            result += symbol;
        }
        file.Seek(save_pos);
    }
    else
    {
        file.Read(buf4);
        for (size_t i = 0; i < nComponents; ++i)
        {
            result += buf4[i];
        }

    }
    return result;
}

// uint16_t
std::string Type3Handler::Handle(InBinFile &file, size_t offset)
{
    bytes addres(4);
    bytes buf2(2);
    bytes buf4(4);
    file.Read(buf4);
    int nComponents = MergeBytes(buf4);
    std::string result;
    if (nComponents * sizeComponent > 4)
    {
        file.Read(addres);

        size_t save_pos = file.Tell();

        file.Seek(offset + MergeBytes(addres));

        for (size_t i = 0; i < nComponents; ++i)
        {
            file.Read(buf2);
            result += MergeBytes(buf2);
            if (i != nComponents - 1)
            {
                // пробел между 16-байтными значениями
                result += ' ';
            }
        }
        file.Seek(save_pos);
    }
    else
    {
        file.Read(buf2);
        file.Seek(2, StartPoint::CUR);
        result = std::to_string(MergeBytes(buf2));
    }
    return result;
}

// uint32_t / uint32_t
std::string Type5Handler::Handle(InBinFile &file, size_t offset)
{
    bytes addres(4);
    bytes buf4(4);

    file.Read(buf4);
    int nComponents = MergeBytes(buf4);
    file.Read(addres);
    // Т.к. будем смещаться, сохраняем текущую позицию
    size_t save_pos = file.Tell();
    file.Seek(offset + MergeBytes(addres));
    std::string result;
    uint32_t values[2];
    for (size_t i = 0; i < nComponents; ++i)
    {
        // Читаем два uint32_t
        for (size_t j = 0; j < 2; j++)
        {
            file.Read(buf4);
            values[j] = MergeBytes(buf4);
        }
        result += std::to_string((double)values[0] / values[1]);
        if (i != nComponents - 1)
        {
            result += ' ';
        }
    }
    // Возвращаемся на текущую позицию
    file.Seek(save_pos);

    return result;
}

void ManagerHandlerType::Handle(int typeDataFormat, std::string &rawProperty, InBinFile &file, size_t offset)
{
    for (auto &&handler : vecHandlers)
    {
        if (handler->ShouldHandle(typeDataFormat))
        {
            rawProperty = handler->Handle(file, offset);
            break;
        }
    }
}

void ManagerHandlerProperty::Handle(uint16_t tag, std::string &property)
{
    for (auto &&handler : vecHandlers)
    {
        if (handler->ShouldHandle(tag))
        {
            handler->Handle(property);
            break;
        }
    }
}

void ExposureTimeHandler::Handle(std::string &property)
{
    puts("ExposureTimeHandler");
    double value = std::round(1.0 / stod(property, nullptr));
    property = toStringWithoutZeros(value);
}

void ApertureValueHandler::Handle(std::string &property)
{
    puts("ApertureValueHandler");
    double value = std::round(stod(property, nullptr) * 10) / 10;
    property = toStringWithoutZeros(value);
}

void FocalLengthHandler::Handle(std::string &property)
{
    puts("FocalLengthHandler");
    double value = std::round(stod(property, nullptr));
    property = toStringWithoutZeros(value);
}

void LatitudeLongtitudeRefHandler::Handle(std::string &property)
{
    puts("LatitudeLongtitudeRefHandler");
    switch(property[0])
    {
        case 'N':
        {
            property = "Север";
            break;
        }
        case 'S':
        {
            property = "Юг";
            break;
        }
        case 'E':
        {
            property = "Восток";
            break;
        }
        case 'W':
        {
            property = "Запад";
            break;
        }
    }
}

void LatitudeLongtitudeHandler::Handle(std::string &property)
{
    puts("LatitudeLongtitudeRefHandler");
    std::replace(property.begin(), property.end(), ',', '.');
    std::stringstream stream(property);
    double value = 0;
    for (size_t i = 0; i < 3; ++i)
    {
        double tmp_value;
        stream >> tmp_value;
        tmp_value /= std::pow(60, i);
        value += tmp_value;
    }
    property = toStringWithoutZeros(value);
}

void DateTimeHandler::Handle(std::string &property)
{
    puts("DateTimeHandler");
    std::replace(property.begin(), property.begin() + 10, ':', '.');
}