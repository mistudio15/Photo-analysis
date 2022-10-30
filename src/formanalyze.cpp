#include <QPushButton>
#include <QTableWidget>

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
    QObject::connect(ui->buttonGraph, &QPushButton::clicked, this, &FormAnalyze::ShowFormGraph);    
    QObject::connect(ui->button_Export, &QPushButton::clicked, this, &FormAnalyze::Export);


    ui->buttonGPS->setEnabled(false);
    ui->buttonGraph->setEnabled(false);

    ui->button_Export->setToolTip("Экспортировать таблицу в формате CSV");
}

void FormAnalyze::Export()
{
    std::cout << "hello";
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

void FormAnalyze::ShowFormGraph()
{
    /*
        опираясь на построенную таблицу построить графики
        1. нужно организовать ввод двух переменных - столбцов (y и x)
        2. если один из столбков текстовый - построить диаграммы или вывести ошибку
        2. нужно вытащить значения из таблицы
    */
    formGraph = new FormGraph(ui->tableWidget);
    formGraph->show();
}

FormAnalyze::~FormAnalyze()
{
    delete ui;
}
