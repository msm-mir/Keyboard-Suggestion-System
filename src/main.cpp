#include <iostream>
#include <QDir>
#include <QRegularExpression>

using namespace std;

bool editFiles(QString folder);
bool openFilesReadOnly(QDir dir, QString fileName, QString &fileContent);
bool openFilesWriteOnly(QDir dir, QString fileName, QString &fileContent);
void toLowerCase(string word);

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
    void setChildren(Node *child, int idx) {
        this->children[idx - 'a'] = child;
    }
    void setLetter(char letter) {
        this->letter = letter;
        this->word += letter;
    }
    void setFileNames(QString fileName) {
        this->fileNames.append(fileName);
    }

    //getter
    Node* getParent() {
        return this->parent;
    }
    Node* getChildren(int idx) {
        return this->children[idx - 'a'];
    }
    char getLetter() {
        return this->letter;
    }
    string getWord() {
        return this->word;
    }
    QStringList getFileNames() {
        return this->fileNames;
    }

};

class Tree {
private:
    Node *root;

public:
    Tree() {
        this->root = nullptr;
    }

    //setter
    void setRoot(Node *root) {
        this->root = root;
    }

    //getter
    Node* getRoot() {
        return this->root;
    }

    void insert(string word, QString fileName) {
        Node *root = this->root;

        if (root == nullptr) {
            Node *newNode = new Node();
            this->root = newNode;
        }

        int i = 0;
        while (i != (int)word.length()) {
            if (root->getChildren(word[i]) == nullptr) {
                Node *newNode = new Node();
                newNode->setParent(root);
                newNode->setLetter(word[i]);
                newNode->setFileNames(fileName);

                root->setChildren(newNode, word[i]);
            }

            root = root->getChildren(word[i]);
            i++;
        }
    }
};

int main() {
    string folder;
    cin >> folder;

    if (editFiles(QString::fromStdString(folder))) {
        Tree tree;
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
