#include <QPushButton>
#include <QTableWidget>
#include <QTableView>
#include <QMessageBox>

#include "formanalyze.h"
#include "ui_formanalyze.h"

FormAnalyze::FormAnalyze(size_t rows, size_t cols, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormAnalyze) 
// выдяем память под виджет (суммарный размер всех указателей на дочерние виджеты)
{
    // запускаем метод, создающий все дочерние для себя (ui) виджеты
    // родительский объект для ui - этот объект (this)
    ui->setupUi(this); 
    ui->tableWidget->setRowCount(rows);
    ui->tableWidget->setColumnCount(cols);

    ui->tableWidget->setSortingEnabled(true);

    QObject::connect(ui->button_Export, &QPushButton::clicked, this, &FormAnalyze::Export);


    ui->buttonGPS->setEnabled(false);
    ui->buttonGraph->setEnabled(false);

    ui->button_Export->setToolTip("Экспортировать таблицу в формате CSV");
}

void FormAnalyze::SetOptimalSize()
{
    ui->tableWidget->setVisible(false);
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->setVisible(true);  

    for (size_t i = 0; i < ui->tableWidget->columnCount(); ++i)
    {
        ui->tableWidget->setColumnWidth(i, ui->tableWidget->columnWidth(i) + 30);
    }
}

void FormAnalyze::Export()
{
    time_t now = time(0);
    std::tm *structTm = localtime(&now);
    std::stringstream date;
    char sep = '_';
    date 
        << 1900 +   structTm->tm_year 
        << sep
        << 1 +      structTm->tm_mon 
        << sep
        <<          structTm->tm_mday
        << sep
        <<          structTm->tm_hour
        << sep
        << 1 +      structTm->tm_hour
        << sep
        << 1 +      structTm->tm_min
        << sep
        << 1 +      structTm->tm_sec;
    std::string file_name = "extracted_data_" + date.str() + ".csv"; 
    std::filesystem::create_directories("../artifacts");
    std::ofstream stream("../artifacts/" + file_name, std::ios::out);

    for (size_t j = 0; j < ui->tableWidget->columnCount(); ++j)
    {
        stream << ui->tableWidget->horizontalHeaderItem(j)->text().toStdString();
        if (j + 1 != ui->tableWidget->columnCount())
        {
            stream << ',';
        }
    }
    stream << '\n';
    for (size_t i = 0; i < ui->tableWidget->rowCount(); ++i)
    {
        for (size_t j = 0; j < ui->tableWidget->columnCount(); ++j)
        {
            stream << ui->tableWidget->item(i, j)->text().toStdString();
            if (j + 1 != ui->tableWidget->columnCount())
            {
                stream << ',';
            }
        }
        stream << '\n';
    }
    stream.flush();
    stream.close();

    QMessageBox::information(this, "Информация", 
                    "Был экспортирован файл " + QString::fromStdString(file_name) + " в каталог artifacts");
}

void FormAnalyze::AddRow(size_t row, std::vector<std::string> const &vec)
{
    for (size_t col = 0; col < vec.size(); ++col)
    {
        QString text = QString::fromStdString(vec[col]);
        QTableWidgetItem *item = new QTableWidgetItem(text);
        ui->tableWidget->setItem(row, col, item);
    }
}

void FormAnalyze::SetHorizontalHeaders(std::vector<std::string> const &vecHeaders)
{
    QStringList stringList;
    for (size_t i = 0; i < vecHeaders.size(); ++i)
    {
        stringList << QString::fromStdString(vecHeaders[i]);
    }
    ui->tableWidget->setHorizontalHeaderLabels(stringList);
}

void FormAnalyze::SetItem(int row, int col, QTableWidgetItem *item)
{
    ui->tableWidget->setItem(row, col, item);
}

FormAnalyze::~FormAnalyze()
{
    delete ui;
}
