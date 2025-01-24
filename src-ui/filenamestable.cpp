#include "filenamestable.h"

FileNamesTable::FileNamesTable(QStringList &fileNames, QString &dir, QWidget *parent) : QWidget(parent), dirPath(dir) {
    table = new QTableWidget(this);
    int rows = std::ceil(std::sqrt(fileNames.size()));
    table->setRowCount(rows);
    table->setColumnCount(rows);

    int index = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < rows; ++j) {
            if (index < fileNames.size()) {
                QTableWidgetItem *item = new QTableWidgetItem(fileNames[index++]);
                table->setItem(i, j, item);
            } else {
                table->setItem(i, j, new QTableWidgetItem(""));
            }
        }
    }

    connect(table, SIGNAL(cellClicked), this, SLOT(onCellClicked()));
}

void FileNamesTable::onCellClicked(int row, int column) {
    QTableWidgetItem *item = table->item(row, column);

    if (item && !item->text().isEmpty()) {
        QString filePath = dirPath + "/" + item->text();
        QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
    }
}
