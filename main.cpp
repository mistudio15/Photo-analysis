#include "input_bin_file.h"
#include "extract_exif.h"

/*
    1. Нужно декомпозировать функцию ExtractExif
    2. "Декорировать" функциональность по чтению байт с учетом Big-endian и Little-endian
    3. Использовать визиторы или цепочки ответственностей
    4. Клиент задает ЛЮБЫЕ теги, программа ищет данные (включая теги не Sub IFD) 
*/

int main()
{
    SettedTags setTags;
    setTags.flags = Tags::ISO | Tags::ExposureTime | Tags::ApertureValue | Tags::FocalLength;
    // With Exif
    // InBinFile file("../slud.jpg");
    InBinFile file("../DSC07634.jpeg");
    EndianDecorator endianFile(std::move(file));
    // Without Exif
    // InBinFile file("../ratibor.png"); 
    auto m = ExtractExif(endianFile, setTags);
    if (m.size() == 0)
    {
        std::cout << "Can not extract exif" << std::endl;
        return 1;
    }
    for (auto const &[key, value] : m)
    {
        std::cout << /* operator << for Tags */ key << " : " << value << std::endl;
    }
    return 1;
}