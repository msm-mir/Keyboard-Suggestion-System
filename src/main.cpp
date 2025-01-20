#include <iostream>
#include <QDir>
#include <QRegularExpression>

using namespace std;

bool editFiles(QString folder);
bool openFilesReadOnly(QDir dir, QString fileName, QString &fileContent);
bool openFilesWriteOnly(QDir dir, QString fileName, QString &fileContent);

int main() {
    string folder;
    cin >> folder;

    if (editFiles(QString::fromStdString(folder))) {

    } else return 0;
}

bool editFiles(QString folder) {
    QDir dir("C:/Users/bpc/Desktop/" + folder);
    QStringList fileNames = dir.entryList(QDir::Files);
    QString fileContent;

    for (QString i : fileNames) {
        openFilesReadOnly(dir, i, fileContent);
        openFilesWriteOnly(dir, i, fileContent);
    }

    return true;
}
bool openFilesReadOnly(QDir dir, QString fileName, QString &fileContent) {
    QFile file(dir.filePath(fileName));

    if (!file.open(QFile::ReadOnly)) {
        cout << "Cannot open file";
        return false;
    }

    fileContent = file.readAll();
    fileContent.remove(QRegularExpression("[^a-zA-Z ]+"));

    file.close();

    return true;
}
bool openFilesWriteOnly(QDir dir, QString fileName, QString &fileContent) {
    QFile file(dir.filePath(fileName));

    if (!file.open(QFile::WriteOnly)) {
        cout << "Cannot open file for writing";
        return false;
    }

    QTextStream out(&file);
    out << fileContent;

    file.close();

    return true;
}
