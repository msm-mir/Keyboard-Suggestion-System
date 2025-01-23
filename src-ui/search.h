#ifndef SEARCH_H
#define SEARCH_H

#include "tree.h"

#include <QMainWindow>
#include <QDir>
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
    Tree tree;

    Search(QWidget *parent = nullptr);
    ~Search();

    void connections();
    void error(QString, bool);
    void openFilesReadOnly(QString, QString&);
    void openFilesWriteOnly(QString, QString);
    void toLowerCase(string&);
    QStringList fileNamesByCondition(QStringList, QStringList, QStringList, QStringList, QStringList, QDir);
    QStringList findCommonElements(QStringList, QStringList);
    QStringList removeCommonElements(QStringList, QStringList, QStringList, QStringList, QDir);
    void printFileNames(QStringList);

private slots:
    void editFiles();

private:
    Ui::Search *ui;
};
#endif // SEARCH_H
