#ifndef SEARCH_H
#define SEARCH_H

#include "tree.h"

#include <QMainWindow>
#include <QDir>
#include <QFileDialog>
#include <QRegularExpression>
#include <QString>
#include <QList>
#include <QListWidgetItem>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class Search; }
QT_END_NAMESPACE

class Search : public QMainWindow { Q_OBJECT
public:
    QStringList fileNames, finalFileNames;
    QDir dir;
    Tree tree;

    Search(QWidget *parent = nullptr);
    ~Search();

    void connections();
    void error(QString, bool);
    void progress(int, bool);
    void openFilesReadOnly(QString, QString&);
    void openFilesWriteOnly(QString, QString);
    void toLowerCase(string&);
    QStringList findCommonElements(QStringList, QStringList);
    void createList();
    void printFileNames(QStringList);

private slots:
    void editFiles();
    void onSearchButtonClicked();
    void onFilterButtonClicked();
    void onItemClicked(QListWidgetItem*);

private:
    Ui::Search *ui;
};
#endif // SEARCH_H
