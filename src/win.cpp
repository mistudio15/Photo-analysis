#include <QPushButton>
#include <QString>
#include <QVector>
#include <QMessageBox>
#include <QFileDialog>
#include <QTabWidget>
#include <QDebug>


#include "stdafx.h"

#include "win.h"
#include "ui_win.h"

std::vector<ReportExtraction> AnalyzeDirectoryImages(std::filesystem::path const &directory_path, ExtracterExif &extracterExif);

Win::Win(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Win)
{
    ui->setupUi(this); 

    QVector<int> vec;
    ui->lineEdit_OpenDir->setPlaceholderText("/home/user/photos");
    ui->lineEdit_Refs->setPlaceholderText("0x8825");
    ui->lineEdit_Tags->setPlaceholderText("0x0001 0x0002 0x0003");
    QObject::connect(ui->buttonExit,    &QPushButton::clicked,  this,   &QWidget::close);
    QObject::connect(ui->buttonAnalyze, &QPushButton::clicked,  this,   &Win::showFormAnalyze);
    QObject::connect(ui->buttonOpenDir, &QPushButton::clicked,  this,   &Win::showFileBrowser);
    QObject::connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &Win::closeTab);

    ui->label_HyperLink->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
    ui->label_HyperLink->setOpenExternalLinks(true);
    ui->label_HyperLink->setText("<a href=\"https://www.media.mit.edu/pia/Research/deepview/exif.html\">Документация на стандарт Exif</a>");

    ui->label_HyperLinkListTags->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
    ui->label_HyperLinkListTags->setOpenExternalLinks(true);
    ui->label_HyperLinkListTags->setText("<a href=\"https://exiftool.org/TagNames/EXIF.html\">Список тегов стандарта Exif</a>");

    QPixmap pix("/home/mikhail/qt_projects/Photo-analysis/static/logo_jpg.png");
    ui->label_ImageLogo->setPixmap(pix);
    ui->label_ImageLogo->setScaledContents(true);
    int pixWidth = pix.width();
    int labelWidth = ui->label_ImageLogo->width();
    double factor = (double)labelWidth / pixWidth;
    ui->label_ImageLogo->setFixedWidth(factor * pix.width());
    ui->label_ImageLogo->setFixedHeight(factor * pix.height());


    ui->buttonOpenDir->setToolTip("Выберите директорию с фотографиями для извлечения метаданных");
    ui->label_Tags->setToolTip("Выберите свойства, которых хотите извлечь из фотографий");
    ui->label_Refs->setToolTip(
        "Если теги, которые вы ввели выше, не расположены в следующих каталогах <br>\
        <strong>IFD0, SubIFD, IFD1</strong>,<br>введите теги, ссылающиеся на директории тегов, для поиска искомых тегов в них.<br><br>\
        <i>Теги в шестнадцатеричном формате, например 0x8891, 0x9345</i>"
        // "Введите теги, ссылающиеся на директории тегов, для поиска тегов, которые ввели выше"
    );
    ui->label_OwnTags->setToolTip(
        "Введите интересующие теги<br><br>\
        <i>Теги в шестнадцатеричном формате, например 0x8891, 0x9345</i>"
    );

    ui->tabWidget->removeTab(1);
    // убрать на первом табе крестик для удаления
}

void Win::showFileBrowser()
{
    QString filename = QFileDialog::getExistingDirectory(
        this, 
        "Open directory",
        "/home/mikhail",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );

    ui->lineEdit_OpenDir->setText(filename);
}

void Win::closeTab(int index)
{
    if (index != 0)
    {
        ui->tabWidget->removeTab(index);
    }
    // else message
}

// C:\photos
// C:\photos\noedit
void Win::showFormAnalyze()
{
    std::string path = ui->lineEdit_OpenDir->text().toStdString();
    std::filesystem::path file_directory{path};

    std::vector<uint16_t> vecTags;
    std::vector<uint16_t> vecRefs;
    // теги должны доставаться из QEditLine и QCheckBox
    vecTags.push_back(0x9003); // дата и время
    if (ui->checkBox_ET->isChecked())
    {
        vecTags.push_back(0x829a);
    }
    if (ui->checkBox_ISO->isChecked())
    {
        vecTags.push_back(0x8827);
    }
    if (ui->checkBox_FN->isChecked())
    {
        vecTags.push_back(0x829d);
    }
    if (ui->checkBox_FL->isChecked())
    {
        vecTags.push_back(0x920a);
    }
    if (ui->checkBox_GPS->isChecked())
    {
        vecRefs.push_back(0x8825);
        vecTags.push_back(0x0001);
        vecTags.push_back(0x0002);
        vecTags.push_back(0x0003);
        vecTags.push_back(0x0004);
    }

    // возможно можно так: streamTags >> std::hex >> tag;
    auto fillFromLineEdit = [](std::string const &tags, std::vector<uint16_t> &vec){
        if (!tags.empty())
        {
            std::stringstream streamTags(tags);
            std::string strTag;
            while (streamTags >> strTag)
            {
                uint16_t tag = (uint16_t)std::stoi(strTag, nullptr, 16);
                vec.push_back(tag);
            }  
        }    
    };
    
    fillFromLineEdit(ui->lineEdit_Tags->text().toStdString(), vecTags);
    fillFromLineEdit(ui->lineEdit_Refs->text().toStdString(), vecRefs);
   

    extracterExif.AddTags(vecTags);
    extracterExif.AddRefs(vecRefs);

    std::vector<ReportExtraction> vecReports = AnalyzeDirectoryImages(file_directory, extracterExif);

    // возможно стоит в цикле заполнения строк всталять новые строки
    // tableWidget->insertRow( tableWidget->rowCount() );
    
    size_t nRows = 0;
    for (size_t i = 0; i < vecReports.size(); ++i)
    {
        if (vecReports[i].done)
        {
            nRows++;
        }
    }

    formAnalyze = new FormAnalyze(nRows, vecTags.size() + 1);

    // Форма с таблицей извлеченных метаданных
    // + 1 колонка для заголовка
    // formAnalyze = new FormAnalyze(vecReports.size(), vecTags.size() + 1);
    // те отчеты в которых done = false входят в вектор, so для них резервируется место в таблице


    // Заголовки ("title" + названия метаданных)
    std::vector<std::string> vecHeaders(vecTags.size() + 1);
    vecHeaders[0] = "Tittle";

    // Заполнение вектора заголовков 
    for (size_t i = 0; i < vecTags.size(); ++i)
    {
        std::cout << "hex = " << std::hex << vecTags[i] << std::endl;
        auto it = ExtracterExif::mapTagsName.find(vecTags[i]); // обращаемся к static const полю
        if (it != ExtracterExif::mapTagsName.end())
        {
            puts("y");
            vecHeaders[i + 1] = ExtracterExif::mapTagsName.at(vecTags[i]);
        }
        else
        {
            puts("n");
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
    size_t row = 0;
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
                // пустая строка, если не нашел 
                vecMetadata.push_back(vecReports[i].mapData[vecTags[j]]);
            }
            formAnalyze->AddRow(row, vecMetadata);
            ++row;
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
    ui->tabWidget->addTab(formAnalyze, "Graph");
    // formAnalyze->show();
}

Win::~Win()
{
    delete ui;
}

std::vector<ReportExtraction> AnalyzeDirectoryImages(std::filesystem::path const &directory_path, ExtracterExif &extracterExif)
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
