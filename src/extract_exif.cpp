#include "extract_exif.h"
#include "input_bin_file.h"

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
    switch(tag)
    {
        case ExposureTime:
        {
            out << "ExposureTime";
            break;
        }
        case ISO:
        {
            out << "ISO";
            break;
        }
        case DateTimeOriginal:
        {
            out << "DateTimeOriginal";
            break;
        }
        case ApertureValue:
        {
            out << "ApertureValue";
            break;
        }
        case FocalLength:
        {
            out << "FocalLength";
            break;
        }
    }
    return out;
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

bool isEqual(bytes const &left, bytes const &right)
{
    return left == right; 
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

// uint16_t
double Type3Handler::Handle(InBinFile &file)
{
    bytes buf2(2);
    /*
        Считаем, что количество компонентов 1 => 1 * 2 (байта) = 2 < 4, 
        значит без смещения
    */
    // Читаем данные
    file.Read(buf2);
    file.Seek(2, StartPoint::CUR);
    return MergeBytes(buf2);
}

// uint32_t / uint32_t
double Type5Handler::Handle(InBinFile &file)
{
    bytes addres(4);
    bytes buf4_1(4);
    bytes buf4_2(4);
    /*
        Считаем что количество компонентов ненулевое => 8 * x > 4, 
        значит со смещением
    */
    file.Read(addres);
    // Т.к. будем смещаться, сохраняем текущую позицию
    size_t save_pos = file.Tell();
    file.Seek(m_offset + MergeBytes(addres));
    // Читаем два uint32_t
    file.Read(buf4_1);
    file.Read(buf4_2);
    // Возвращаемся на текущую позицию
    file.Seek(save_pos);

    return static_cast<double>(MergeBytes(buf4_1)) / MergeBytes(buf4_2);
}

// устанавливает файловый указатель на TIFF Header
bool HasExif(InBinFile &file)
{
    // bytes SOI  - начальный маркер JPEG
    // bytes APP1 - маркер заголовка APP1     

    bytes SOI(2);
    bytes APP1(2);
    bytes ExifHeader(cExifHeader.size());
    
    file.Read(SOI);
    file.Read(APP1);
    // пропускаем APP1 Length
    file.Seek(2, StartPoint::CUR);
    file.Read(ExifHeader);

    if (!isEqual(SOI, cSOI) 
        || !isEqual(APP1, cAPP1)  
        || !isEqual(ExifHeader, cExifHeader))
    {
        return false;
    }
    return true;
}

// Пустой контейнер std::map на выходе соответствует отсутствию либо Exif, 
// либо установленных признаков
std::unordered_map<uint32_t, double> ExtractExif(InBinFile &file, std::vector<bytes> const &vecTags)
{
    // буферы для чтения из бинарного файла
    bytes buf2(2);
    bytes buf4(4);
    if (!HasExif(file))
    {
        return {};
    }
    /*
    Позиция, с которой начинается отсчет при любых смещениях
    */
    size_t offset = file.Tell();
    /*
        "First 8bytes of TIFF format are TIFF header. 
        First 2bytes defines byte align of TIFF data: 
        "II" (Intel) means Little-endian, "MM" (Motorola) means Big-endian"
    */
    file.Read(buf2);
    if (buf2 == cByteAlignII)
    {
        // небольшой костыль...
        // т.к. порядок байтов становится известен только на этапе просмотра
        (dynamic_cast<EndianDecorator&>(file)).SetEndian(Endian::LITTLE);
    }
    // else Endian::BIG by default

    // Пропускаем 2A 00 (Little) или 00 2A (Big)
    bool f = file.Seek(2, StartPoint::CUR);
    // смещение к IFD0
    file.Read(buf4);
    file.Seek(offset + MergeBytes(buf4));
    // Количество записей по 12 байт
    file.Read(buf2);
    bool isExifOffset = false;
    size_t nEntries = MergeBytes(buf2);
    /*
        Поиск тега ExifOffset, который хранит смещение к Exif SubIFD,
        где расположены теги, соответствующие настройкам камеры. 
    */
    for (size_t i = 0; i < nEntries; ++i)
    {
        file.Read(buf2);
        if (buf2 == cExifOffset)
        {
            isExifOffset = true;
            break;
        }
        // Переходим на следующую запись
        file.Seek(10, StartPoint::CUR);
    } 
    if (!isExifOffset)
    {
        return {};
    }
    /*
        Пропускаем формат данных и количество компонентов тега ExifOffset,
        т.к. они известны: формат данных - unsigned int (4), количество компонентов - 1.
    */
    file.Seek(6, StartPoint::CUR);
    // Читаем смещение к Exif SubIFD
    file.Read(buf4);
    std::unordered_map<uint32_t, double> table;
    // Почему + 2 не понял
    file.Seek(offset + MergeBytes(buf4) + 2);
    // Читаем тэг
    file.Read(buf2);
    // просмотр каждой записи и поиск соответствующих установленным флагам
    // конец цикла - нулевой тэг

    Type3Handler h3(offset);
    Type5Handler h5(offset);
    std::vector<Handler *> vecHandlers = {&h3, &h5};
    
    while (MergeBytes(buf2) != 0)
    {
        bool found = false;
        bytes curTag(2);

        auto it = std::find(vecTags.begin(), vecTags.end(), buf2);
        if (it != vecTags.end())
        {
            found = true;
            curTag = buf2;
        }
        if (!found)
        {
            file.Seek(10, StartPoint::CUR);
        }
        else
        {
            // Читаем и сохраняем код формата данных
            file.Read(buf2);
            int typeDataFormat = MergeBytes(buf2);
            // Читаем и сохраняем количество компонентов (не используется)
            file.Read(buf4);
            int nComponents = MergeBytes(buf4);
            /*
                ExposureTime        5 = uint32_t / uint32_t
                ISO                 3 = uint16_t
                DateTimeOriginal    2 = uint8_t (ASCII)
                ApertureValue       5
                FocalLength         5
            */

            for (auto &&handler : vecHandlers)
            {
                if (handler->ShouldHandle(typeDataFormat))
                {
                    table[MergeBytes(curTag)] = handler->Handle(file);
                    break;
                }
            }

        }
        file.Read(buf2);
    }
    // Обработка значений выдержки и диафрагмы 
    table[MergeBytes(cExposureTime)]   = 1.0 / table[MergeBytes(cExposureTime)];
    table[MergeBytes(cApertureValue)]  = pow(1.4142, table[MergeBytes(cApertureValue)]);
    return table;
}