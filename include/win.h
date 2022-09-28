#ifndef WIN_H
#define WIN_H

#include <QWidget>

#include "formanalyze.h"
#include "extract_exif.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Win; }
QT_END_NAMESPACE

// std::vector<ReportExtraction> AnalyzeDirectoryImages(std::filesystem::path const &directory_path, ExtracterExif &extracterExif);

// /media/mikhail/8E54B23D54B2283B/photos
// /media/mikhail/8E54B23D54B2283B/photos
//media/mikhail/8E54B23D54B2283B/МГТУ/_Технопарк/C_С++/Летние_тренировки/bin_file/photos
// /home/mikhail/Qt/myQtProjects/photos

class Win : public QWidget
{
    Q_OBJECT

public:
    Win(QWidget *parent = nullptr);
    ~Win();
public slots:
    void showFormAnalyze();

private:
    Ui::Win *ui;
    FormAnalyze *formAnalyze = nullptr;
    ExtracterExif extracterExif;
    // интересующие пользователя теги
    // std::vector<uint16_t> vecTags = { 0x829A, 0x8827, 0x920A, 0x829d, 0x9003, 0x0001, 0x0003 };
};
#endif // WIN_H
