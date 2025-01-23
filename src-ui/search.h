#ifndef SEARCH_H
#define SEARCH_H

#include <QMainWindow>
#include <QDir>
#include <QPushButton>
#include <QFileDialog>
#include <QRegularExpression>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class Search; }
QT_END_NAMESPACE

class Search : public QMainWindow { Q_OBJECT
public:
    QStringList fileNames;
    QDir dir;

    Search(QWidget *parent = nullptr);
    ~Search();

    void connections();
    bool openFilesReadOnly(QDir, QString, QString&);
    bool openFilesWriteOnly(QDir, QString, QString);
    void toLowerCase(string&);
    QStringList fileNamesByCondition(QStringList, QStringList, QStringList, QStringList, QStringList, QDir);
    QStringList findCommonElements(QStringList, QStringList);
    QStringList removeCommonElements(QStringList, QStringList, QStringList, QStringList, QDir);
    void printFileNames(QStringList);

private slots:
    bool editFiles();

private:
    Ui::Search *ui;
};
#endif // SEARCH_H
