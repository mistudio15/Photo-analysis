    #ifndef FORMGRAPH_H
#define FORMGRAPH_H

#include <QWidget>
#include <QTableWidget>

#include "stdafx.h"

namespace Ui {
class FormGraph;
}

class FormGraph : public QWidget
{
    Q_OBJECT

public:
    explicit FormGraph(QTableWidget *tableWidget, QWidget *parent = nullptr);
    ~FormGraph();
public slots:
    void BuildGraph();
    // void ActivatedComboBox(int index); 
private:
    Ui::FormGraph *ui;
    QTableWidget *tableWidget = nullptr;
};

#endif // FORMGRAPH_H
