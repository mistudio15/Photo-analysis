#include "formgraph.h"
#include "extract_exif.h"
#include "ui_formgraph.h"

#include <QVector>
#include <QDebug>


FormGraph::FormGraph(QTableWidget *tableWidget, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormGraph),
    tableWidget(tableWidget)
{
    ui->setupUi(this);
    for (size_t i = 1; i < tableWidget->columnCount(); ++i)
    {
        // от i = 1, потому что 0-й item - это tittle
        std::cout << tableWidget->horizontalHeaderItem(i)->text().toStdString() << std::endl;
        ui->comboBox_x->addItem(tableWidget->horizontalHeaderItem(i)->text());
        ui->comboBox_y->addItem(tableWidget->horizontalHeaderItem(i)->text());
    }
    QObject::connect(ui->buildGraphButton, &QPushButton::clicked, this, &FormGraph::BuildGraph);
}

void FormGraph::BuildGraph()
{
    std::setlocale(LC_NUMERIC, "POSIX"); // не знаю, где выставляется локаль, где вещественное число с запятой...
    ui->widget->clearGraphs();
    QVector<double> vecX, vecY;
    for (size_t i = 0; i < tableWidget->rowCount(); ++i)
    {
        std::cout << "__" << ui->comboBox_x->currentText().toStdString()  << std::endl;
        if (ui->comboBox_x->currentText().toStdString() == "DateTimeOriginal")
        {
            puts("if true");
            QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);

            dateTicker->setDateTimeFormat("yyyy:mm:dd hh:mm:ss");

            ui->widget->xAxis->setTicker(dateTicker);

//            ui->widget->xAxis->setTickLabelType(QCPAxis::ltDateTime);
//            ui->widget->xAxis->setDateTimeFormat("yyyy:mm:dd hh:mm:ss");

            std::string date = tableWidget->item(i, ui->comboBox_x->currentIndex() + 1)->text().toStdString();
            int h, m, s, y, min, d;
            struct std::tm when = {0};

            sscanf(date.c_str(), "%d:%d:%d %d:%d:%d", &y, &m, &d, &h, &min, &s);

            when.tm_year = y - 1900;
            when.tm_mon = m - 1;
            when.tm_mday = d;
            when.tm_hour = h;
            when.tm_min = min;
            when.tm_sec = s;

            std::time_t convertedTime;
            convertedTime = std::mktime(&when);

            vecX.push_back(convertedTime);
        }   
        else 
        {
            vecX.push_back(stod(tableWidget->item(i, ui->comboBox_x->currentIndex() + 1)->text().toStdString(), nullptr));
        }

        vecY.push_back(stod(tableWidget->item(i, ui->comboBox_y->currentIndex() + 1)->text().toStdString(), nullptr));
    }
    qDebug() << vecX;
    qDebug() << vecY;

    ui->widget->xAxis->setRange(*std::min_element(vecX.begin(), vecX.end()), *std::max_element(vecX.begin(), vecX.end()));
    ui->widget->yAxis->setRange(0, *std::max_element(vecY.begin(), vecY.end()));

    ui->widget->addGraph()->setData(vecX, vecY);
    ui->widget->replot();
}

// void FormGraph::ActivatedComboBox(int index)
// {
//     QComboBox *comboBox = qobject_cast<QComboBox *>(QObject::sender());
//     if (comboBox == ui->comboBox_x)
//     {
//         ui->comboBox_y->removeItem(index);
//     }
//     else if (comboBox == ui->comboBox_y)
//     {
//         ui->comboBox_x->removeItem(index);
//     }
// }

FormGraph::~FormGraph()
{
    delete ui;
}


