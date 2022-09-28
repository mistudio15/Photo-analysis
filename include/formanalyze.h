#ifndef FORMANALYZE_H
#define FORMANALYZE_H

#include <QWidget>

#include "extract_exif.h"
#include "formgraph.h"

namespace Ui {
class FormAnalyze;
}

class FormAnalyze : public QWidget
{
    Q_OBJECT

public:
    explicit FormAnalyze(size_t rows, size_t cols, QWidget *parent = nullptr);
    // разве не лучше FillRow ? 
    void AddRow(size_t row, std::vector<std::string> const &vec);
    void SetHorizontalHeaders(std::vector<std::string> const &vecHeaders);
    ~FormAnalyze();
public slots:
    void ShowFormGraph();

private:
    Ui::FormAnalyze *ui;
    FormGraph *formGraph = nullptr;
};

#endif // FORMANALYZE_H
