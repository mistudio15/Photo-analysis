#include "include/extract_exif.h"

using std::cout;
using std::endl;

const bytes cSOI                = { 0xFF, 0xD8 };
const bytes cAPP1               = { 0xFF, 0xE1 };
const bytes cExifHeader         = { 0x45, 0x78, 0x69, 0x66, 0x00, 0x00 };
const bytes cByteAlignMM        = { 0x4d, 0x4d };
const bytes cByteAlignII        = { 0x49, 0x49 };

const bytes cExifOffset         = { 0x87, 0x69 };

const bytes cExposureTime       = { 0x82, 0x9a };
const bytes cISO                = { 0x88, 0x27 };
const bytes cDateTimeOriginal   = { 0x90, 0x03 };
const bytes cApertureValue      = { 0x92, 0x02 };
const bytes cFocalLength        = { 0x92, 0x0A };

std::ostream &operator <<(std::ostream &out, Tags const &tag)
{
    std::string nameEnum;
#define PRINT(a) nameEnum = #a;
    switch(tag)
    {
        case ExposureTime:
        {
            PRINT(ExposureTime);
            break;
        }
        case ISO:
        {
            PRINT(ISO);
            break;
        }
        case DateTimeOriginal:
        {
            PRINT(DateTimeOriginal);
            break;
        }
        case ApertureValue:
        {
            PRINT(ApertureValue);
            break;
        }
        case FocalLength:
        {
            PRINT(FocalLength);
            break;
        }
    }
    return out << nameEnum;
}

void print(bytes const &vec, std::string message = "")
{
    std::cout << message << std::endl;
    for (auto &&elem : vec)
    {
        std::cout << std::hex << (int)elem << " ";
    }
    std::cout << std::endl;
}

void ReverseBytes(bytes &bytes)
{
    std::reverse(bytes.begin(), bytes.end());
}

uint64_t MergeBytes(bytes const &vecBytes)
{
    assert(vecBytes.size() > 0);
    uint64_t result = 0;
    size_t power = 0;
    for (auto rIt = vecBytes.rbegin(); rIt != vecBytes.rend(); ++rIt)
    {
        if (*rIt != 0)
        {
            result += *rIt * pow(256, power);
        }
        ++power;
    }
    return result;
}

// C:\photos
ExtracterExif::ExtracterExif(std::vector<uint16_t> const &tags) : vecTags(tags)
{
    vecHandlers.push_back(std::make_unique<Type2Handler>());
    vecHandlers.push_back(std::make_unique<Type3Handler>());
    vecHandlers.push_back(std::make_unique<Type5Handler>());
}

ReportExtraction ExtracterExif::ExtractExif(InBinFile &file)
{
    report.Clear();
    report.file_name = file.GetFileName();
    if (!HasExif(file))
    {
        report.done = false;
        return report;
    }
    report.done = true;
    // Позиция, с которой начинается отсчет при любых смещениях
    offset = file.Tell();
    // "II" (Intel) - Little-endian, "MM" (Motorola) = Big-endian
    file.Read(buf2);
    if (buf2 == cByteAlignII)
    {
        (dynamic_cast<EndianFile&>(file)).SetEndian(Endian::LITTLE);
    }
    // else Endian::BIG by default

    // Пропускаем 2A 00 (Little) или 00 2A (Big)
    file.Seek(2, StartPoint::CUR);

    // смещение к IFD0
    file.Read(buf4);
    /*
        Парсинг каталогов (IFD0, SubIFD, IFD1, ...)
        и вложенных директорий со специализированными тегами
        (GPS Tags, Photoshop Tags, ...)
        Ссылки на такие директории в vecRefs<uint16_t>
    */
    while (MergeBytes(buf4) != 0)
    {
        file.Seek(offset + MergeBytes(buf4));
        Parse(file);
        // Смещение к следующему каталогу
        file.Read(buf4);
    }
    return report;
}

void ExtracterExif::Parse(InBinFile &file)
{
    // Количество записей
    file.Read(buf2);
    size_t nEntries = MergeBytes(buf2);
    for (size_t i = 0; i < nEntries; ++i)
    {
        // Тэг
        file.Read(buf2);

        // Парсим поддиректорию
        auto itRefs = std::find(vecRefs.begin(), vecRefs.end(), MergeBytes(buf2));
        if (itRefs != vecRefs.end())
        {
            file.Seek(6, StartPoint::CUR);
            // Смещение
            file.Read(buf4);
            size_t save_pos = file.Tell();
            file.Seek(offset + MergeBytes(buf4));

            Parse(file);

            file.Seek(save_pos);
            continue;
        }

        // Тэг записи не установлен
        auto itTags = std::find(vecTags.begin(), vecTags.end(), MergeBytes(buf2));
        if (itTags == vecTags.end())
        {
            // Пропускаем запись
            file.Seek(10, StartPoint::CUR);
        }
        // Тэг записи установлен
        else
        {
            uint16_t curTag = MergeBytes(buf2);
            // Код формата данных
            file.Read(buf2);
            int typeDataFormat = MergeBytes(buf2);
            for (auto &&handler : vecHandlers)
            {
                if (handler->ShouldHandle(typeDataFormat))
                {
                    report.mapData[curTag] = handler->Handle(file, offset);
                    ProcessRawProp(curTag, report.mapData[curTag]);
                    // если знаем как обработать найденный тег, обрабатываем в обработчике
                    break;
                }
            }
        }
    }
}

bool ExtracterExif::HasExif(InBinFile &file)
{
    // bytes SOI  - начальный маркер JPEG
    // bytes APP1 - маркер заголовка APP1

    file.Seek(0);

    bytes SOI(2);
    bytes APP1(2);
    bytes ExifHeader(6);

    file.Read(SOI);
    file.Read(APP1);
    // пропускаем APP1 Length
    file.Seek(2, StartPoint::CUR);
    file.Read(ExifHeader);

    if (    SOI         !=  cSOI            ||
            APP1        !=  cAPP1           ||
            ExifHeader  !=  cExifHeader
       )
    {
        return false;
    }
    return true;
}

// table[MergeBytes(cExposureTime)]   = 1.0 / table[MergeBytes(cExposureTime)];
// table[MergeBytes(cApertureValue)]  = pow(1.4142, table[MergeBytes(cApertureValue)]);
// table[MergeBytes(cApertureValue)]  = std::round(pow(1.4142, table[MergeBytes(cApertureValue)]) * 10) / 10;
// table[MergeBytes(cFocalLength)]  = std::round(table[MergeBytes(cFocalLength)]);
