#include "filenamestable.h"

FileNamesTable::FileNamesTable(QTableWidget *table, QStringList fileNames, QString dir) {
    populateTable(table, fileNames, dir);
}

void FileNamesTable::populateTable(QTableWidget *table, QStringList fileNames, QString dir) {
    if (!table)
        return;

    table->clear();


    int rows, cols = std::ceil(std::sqrt(fileNames.size()));
    if (cols <= 20) {
        rows = fileNames.size() / cols;
    }
    else {
        cols = 20;
        rows = std::ceil((double)fileNames.size() / cols);
    }

    table->setRowCount(rows);
    table->setColumnCount(cols);

    int index = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (index < fileNames.size()) {
                QTableWidgetItem *item = new QTableWidgetItem(fileNames[index]);
                table->setItem(i, j, item);
                ++index;
            }
        }
    }

    QObject::connect(table, &QTableWidget::cellClicked, [dir, table](int r, int c) {
        QTableWidgetItem *clickedItem = table->item(r, c);
        if (clickedItem) {
            QString filePath = QDir(dir).absoluteFilePath(dir + "/" + clickedItem->text());
            QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
        }
    });

    table->resizeColumnsToContents();
    table->resizeRowsToContents();
}
