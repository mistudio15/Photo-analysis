#include "include/formgraph.h"
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
        // QObject::connect(ui->comboBox_x, &QComboBox::activated, this, &FormGraph::ActivatedComboBox);
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
       qDebug() << tableWidget->item(i, ui->comboBox_x->currentIndex() + 1)->text();
       std::string sss = tableWidget->item(i, ui->comboBox_x->currentIndex() + 1)->text().toStdString();

       std::cout << sss << std::endl;


       float value = std::stof(sss, nullptr);
       std::cout << value << std::endl;

       vecX.push_back(stod(tableWidget->item(i, ui->comboBox_x->currentIndex() + 1)->text().toStdString(), nullptr));
       vecY.push_back(stod(tableWidget->item(i, ui->comboBox_y->currentIndex() + 1)->text().toStdString(), nullptr));
    }
    qDebug() << vecX;
    std::cout << vecX[0] << " " << vecX[1] << std::endl;
    qDebug() << vecY;
    std::cout << vecY[0] << " " << vecY[1] << std::endl;
    std::cout << *std::max_element(vecX.begin(), vecX.end()) << " " << *std::min_element(vecX.begin(), vecX.end()) << std::endl;
    std::cout << *std::max_element(vecY.begin(), vecY.end()) << " " << *std::min_element(vecY.begin(), vecY.end()) << std::endl;


//    ui->widget->xAxis->setRange(*std::min_element(vecX.begin(), vecX.end()), *std::max_element(vecX.begin(), vecX.end()));
//    ui->widget->yAxis->setRange(*std::min_element(vecY.begin(), vecY.end()), *std::max_element(vecY.begin(), vecY.end()));

    ui->widget->xAxis->setRange(0, *std::max_element(vecX.begin(), vecX.end()));
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


