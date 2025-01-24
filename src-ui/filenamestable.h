#ifndef FILENAMESTABLE_H
#define FILENAMESTABLE_H

#include <QWidget>
#include <QTableWidget>

class FileNamesTable : public QWidget { Q_OBJECT
public:
    FileNamesTable(QStringList&, QString&, QWidget *parent = nullptr);

private slots:
    void onCellClicked(int, int);

private:
    QTableWidget *table;
    QString dirPath;

};

#endif // FILENAMESTABLE_H
