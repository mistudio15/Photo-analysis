#ifndef WIN_H
#define WIN_H

#include <QWidget>

#include "formanalyze.h"
#include "extract_exif.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Win; }
QT_END_NAMESPACE

class Win : public QWidget
{
    Q_OBJECT

public:
    Win(QWidget *parent = nullptr);
    ~Win();
public slots:
    void showFormAnalyze();
    void showFileBrowser();
    void closeTab(int index);

private:
    Ui::Win *ui;
    ExtracterExif extracterExif;
    FormAnalyze *formAnalyze = nullptr;
    
    void prepareWindow();

    void fillVectorFromCheckBoxes(std::vector<uint16_t> &vecTags, std::vector<uint16_t> &vecRefs);

    bool fillVectorFromLineEdit(std::vector<uint16_t> &vecTags, std::vector<uint16_t> &vecRefs);

    size_t getCountRows(std::vector<ReportExtraction> const &vecReports);

    void fillVectorFields(std::vector<uint16_t> const &vecTags, std::vector<std::string> &vecFields);

    void fillTableWithReports(std::vector<uint16_t> const &vecTags, std::vector<ReportExtraction> const &vecReports);

    void notifyAboutFilesWithoutExif(std::vector<ReportExtraction> const &vecReports);

    void showWidgetInTab(QWidget *widget);
};
#endif // WIN_H

// интересующие пользователя теги
// std::vector<uint16_t> vecTags = { 0x829A, 0x8827, 0x920A, 0x829d, 0x9003, 0x0001, 0x0003 };