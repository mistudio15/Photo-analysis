#include "stdafx.h"
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

std::vector<uint16_t> ReadTagsFromFile(std::string const &file_path)
{
    std::ifstream fileTags(file_path);
    if (!fileTags.is_open())
    {
        std::cout << "Can not find file with tags" << std::endl;
        return {};
    }
    std::vector<uint16_t> vecTags;
    std::string strByte;
    while (fileTags >> strByte)
    {
        uint16_t tag = (uint16_t)std::stoi(strByte, nullptr, 16);
        vecTags.push_back(tag);
    }
    fileTags.close();
    return vecTags;
}

void PrintReport(ReportExtraction const &report, std::ostream &out)
{
    if (!report.done)
    {
        out << "Can not extract exif" << std::endl;
        return;
    }
    if (report.mapData.size() == 0)
    {
        out << "Can not find needed tags" << std::endl;
        return;
    }
    for (auto const &[key, value] : report.mapData)
    {
        out << std::setfill('0') << std::setw(4) << std::hex << (int)key <<  " : " << value << std::endl;
    }
}

int main()
{   
    std::vector<uint16_t> vecTags = ReadTagsFromFile("../tags.txt");

    EndianFile endianFile("../gps.jpg");

    ExtracterExif extracterExif(vecTags); 

    ReportExtraction report = extracterExif.ExtractExif(endianFile);

    PrintReport(report, std::cout);

    return 1;
}