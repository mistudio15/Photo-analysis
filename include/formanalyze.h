#ifndef FORMANALYZE_H
#define FORMANALYZE_H

#include <QWidget>

#include "extract_exif.h"

namespace Ui {
class FormAnalyze;
}

class FormAnalyze : public QWidget
{
    Q_OBJECT

public:
    explicit FormAnalyze(size_t rows, size_t cols, QWidget *parent = nullptr);
    void AddRow(size_t rows, std::vector<std::string> const &vec);
    void SetHorizontalHeaders(std::vector<std::string> const &vecHeaders);
    ~FormAnalyze();

private:
    Ui::FormAnalyze *ui;
};

#endif // FORMANALYZE_H
