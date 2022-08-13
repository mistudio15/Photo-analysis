#include "input_bin_file.h"
#include "extract_exif.h"

/*
    1. Нужно декомпозировать функцию ExtractExif
    2. "Декорировать" функциональность по чтению байт с учетом Big-endian и Little-endian
    3. Использовать визиторы или цепочки ответственностей
    4. Клиент задает ЛЮБЫЕ теги, программа ищет данные (включая теги не Sub IFD) 
*/

/*
    Если в файл теги будут записаны не по одному байту,
    Необходимо после считывания в строку 2-х байт
    извлекать из нее по два символа
    Возможно придется записывать строку в stringstream
    и get(tempStr, 2)
    либо использовать метод строки по "изьятию" 2-х симолов
*/

/*
    Наверное, лучше, чтобы Handler'ы возвращали поток с указателем в том же месте, в каком и принимали
*/

int main()
{
    std::ifstream fileTags("../tags.txt");
    if (!fileTags.is_open())
    {
        std::cout << "Can not find file with tags" << std::endl;
        return 1;
    }
    std::vector<bytes> vecTags;
    std::string strByte;
    bytes Tag(2);
    while (fileTags >> strByte)
    {
        Tag[0] = (byte)std::stoi(strByte, nullptr, 16);

        fileTags >> strByte;
        Tag[1] = (byte)std::stoi(strByte, nullptr, 16);
        
        vecTags.push_back(Tag);
    }
    fileTags.close();

    // SettedTags setTags;
    // setTags.flags = Tags::ISO | Tags::ExposureTime | Tags::ApertureValue | Tags::FocalLength;
    // With Exif
    // InBinFile file("../slud.jpg");
    InBinFile file("../DSC07634.jpeg");
    EndianDecorator endianFile(std::move(file));
    // Without Exif
    // InBinFile file("../ratibor.png"); 

    auto m = ExtractExif(endianFile, vecTags);
    if (m.size() == 0)
    {
        std::cout << "Can not extract exif" << std::endl;
        return 1;
    }
    for (auto const &[key, value] : m)
    {
        std::cout << std::hex <<  key <<  " : " << value << std::endl;
    }
    return 1;
}