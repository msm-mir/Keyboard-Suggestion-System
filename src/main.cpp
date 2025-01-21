#include <iostream>
#include <QDir>
#include <QList>
#include <QRegularExpression>

using namespace std;

bool editFiles(QString folder);
bool openFilesReadOnly(QDir dir, QString fileName, QString &fileContent);
bool openFilesWriteOnly(QDir dir, QString fileName, QString &fileContent);

class Node {
private:
    QStringList fileNames;
    Node *parent;
    Node **children = new Node*[26];
    char *edges = new char[26];

public:
    Node() {
        this->fileNames.clear();
        this->parent = nullptr;

        for (int i = 0; i < 26; i++) {
            this->children[i] = nullptr;
            this->edges[i] = 0;
        }
    }

    void setFileNames(QString fileName) {
        this->fileNames.append(fileName);
    }
    void setParent(Node *parent) {
        this->parent = parent;
    }
    void setChildren(Node *child, char letter) {
        int idx = tolower(letter) - 'a';

        this->children[idx] = child;
        this->edges[idx] = letter;
    }

    QStringList getFileNames() {
        return this->fileNames;
    }
    Node* getParent() {
        return this->parent;
    }
    Node* getChildren(int idx) {
        return this->children[idx];
    }
    char getEdges(int idx) {
        return this->edges[idx];
    }

};

int main() {
    string folder;
    cin >> folder;

    if (editFiles(QString::fromStdString(folder))) {

    } else return 0;
}

bool editFiles(QString folder) {
    QDir dir("C:/Users/bpc/Desktop/" + folder);
    QStringList fileNames = dir.entryList(QDir::Files);

    for (QString i : fileNames) {
        QString fileContent;
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
