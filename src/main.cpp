#include <iostream>
#include <QDir>
#include <QRegularExpression>

using namespace std;

bool editFiles(QString folder);
bool openFilesReadOnly(QDir dir, QString fileName, QString &fileContent);
bool openFilesWriteOnly(QDir dir, QString fileName, QString &fileContent);

class Node {
private:
    Node *parent;
    Node **children;
    char letter;
    string word;
    QStringList fileNames;

public:
    Node() {
        this->parent = nullptr;
        this->children = new Node*[26];
        this->letter = 0;
        this->word = "";
        this->fileNames.clear();

        for (int i = 0; i < 26; i++)
            this->children[i] = nullptr;
    }

    //setter
    void setParent(Node *parent) {
        this->parent = parent;
    }
    void setChildren(Node *child, char letter) {
        this->children[letter - 'a'] = child;
    }
    void setFileNames(QString fileName) {
        this->fileNames.append(fileName);
    }

    //getter
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

//get a list of files' name;
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
//open file to read and edit its text
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
//open file to write the edited text into it
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
