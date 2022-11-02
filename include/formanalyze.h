#ifndef FORMANALYZE_H
#define FORMANALYZE_H

#include <QWidget>

#include "extract_exif.h"
#include <QTableWidgetItem>

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
    void SetOptimalSize();
    void SetItem(int row, int col, QTableWidgetItem *item);
public slots:
    void Export();

private:
    Ui::FormAnalyze *ui;
};

#endif // FORMANALYZE_H
