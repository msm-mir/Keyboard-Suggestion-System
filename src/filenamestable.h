#ifndef FILENAMESTABLE_H
#define FILENAMESTABLE_H

#include <QDir>
#include <QTableWidget>
#include <QDesktopServices>
#include <QUrl>

class FileNamesTable {
public:
    FileNamesTable(QTableWidget*, QStringList, QString);
    void populateTable(QTableWidget*, QStringList, QString);

};

#endif // FILENAMESTABLE_H
