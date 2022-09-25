#include <QPushButton>
#include <QTableWidget>

#include "formanalyze.h"
#include "ui_formanalyze.h"

FormAnalyze::FormAnalyze(size_t rows, size_t cols, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormAnalyze)
{
    ui->setupUi(this);
    ui->tableWidget->setRowCount(rows);
    ui->tableWidget->setColumnCount(cols);
    QObject::connect(ui->buttonExit, &QPushButton::clicked, this, &QWidget::close);
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

FormAnalyze::~FormAnalyze()
{
    delete ui;
}
