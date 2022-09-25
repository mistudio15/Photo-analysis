#include <QPushButton>
#include <qDebug>
#include <QString>
#include <QVector>
#include <QMessageBox>

#include "stdafx.h"

#include "win.h"
#include "ui_win.h"

Win::Win(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Win)
{
    ui->setupUi(this); 
    QObject::connect(ui->buttonExit,    &QPushButton::clicked,  this,   &QWidget::close);
    QObject::connect(ui->buttonAnalyze, &QPushButton::clicked,  this,   &Win::showFormAnalyze);
}

// C:\photos
// C:\photos\noedit
void Win::showFormAnalyze()
{
    std::string path = ui->linePath->text().toStdString();
    std::filesystem::path file_directory{path};

    // теги должны доставаться из QEditLine и QCheckBox
    extracterExif.AddTags(vecTags);

    
    std::vector<ReportExtraction> vecReports = AnalyzeDerectoryImages(file_directory, extracterExif);


    // Форма с таблицей извлеченных метаданных
    // + 1 колонка для заголовка
    formAnalyze = new FormAnalyze(vecReports.size(), vecTags.size() + 1);

    // Заголовки ("title" + названия метаданных)
    std::vector<std::string> vecHeaders(vecTags.size() + 1);
    vecHeaders[0] = "Tittle";

    // Заполнение вектора заголовков 
    for (size_t i = 0; i < vecTags.size(); ++i)
    {
        auto it = ExtracterExif::mapTagsName.find(vecTags[i]);
        if (it != ExtracterExif::mapTagsName.end())
        {
            vecHeaders[i + 1] = ExtracterExif::mapTagsName.at(vecTags[i]);
        }
        else
        {
            std::string tagStr;
            std::stringstream sstream;
            sstream << "0x" << std::setfill('0') << std::setw(4) << std::hex << vecTags[i];
            sstream >> tagStr;
            vecHeaders[i + 1] = tagStr;
        }
    }
    formAnalyze->SetHorizontalHeaders(vecHeaders);

    std::vector<std::string> vecFilesNoExif;
    // Заполнение таблицы
    size_t col = 0;
    for (size_t i = 0; i < vecReports.size(); ++i)
    {
        if (vecReports[i].done == false)
        {   
            vecFilesNoExif.push_back(vecReports[i].file_name);
        }
        else
        {
            std::vector<std::string> vecMetadata;
            vecMetadata.push_back(vecReports[i].file_name);
            for (size_t j = 0; j < vecTags.size(); ++j)
            {
                vecMetadata.push_back(vecReports[i].mapData[vecTags[j]]);
            }
            formAnalyze->AddRow(col, vecMetadata);
            ++col;
        }
    }

    if (!vecFilesNoExif.empty())
    {
        std::string listFilesNoExif = 
                        std::accumulate(std::next(vecFilesNoExif.begin()), vecFilesNoExif.end(), 
                        vecFilesNoExif[0], [](std::string init, std::string const &strFile){
                                return init + ", " + strFile;
                            });

        QMessageBox::warning(this, "Предупреждение", 
                    "Photos [ " + QString::fromStdString(listFilesNoExif) + " ] did not find Exif");
    }
    formAnalyze->show();
}

Win::~Win()
{
    delete ui;
}

std::vector<ReportExtraction> AnalyzeDerectoryImages(std::filesystem::path const &directory_path, ExtracterExif &extracterExif)
{
    std::vector<ReportExtraction> vecReports;
    for (auto const &file : std::filesystem::directory_iterator(directory_path))
    {
        if (file.path().extension().string() == ".jpg" || file.path().extension().string() == ".jpeg")
        {
            EndianFile endianFile(file.path().string());
            vecReports.push_back(extracterExif.ExtractExif(endianFile));
        }
    }
    return vecReports;
}

