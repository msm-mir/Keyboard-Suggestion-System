#include <iostream>
#include <QDir>
#include <QRegularExpression>

using namespace std;

bool editFiles(QDir &dir, QStringList &fileNames, string folder);
bool openFilesReadOnly(QDir dir, QString fileName, QString &fileContent);
bool openFilesWriteOnly(QDir dir, QString fileName, QString fileContent);
void toLowerCase(string &word);

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
    void setChildren(Node *child, char idx) {
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
    Node* getChildren(char idx) {
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
        toLowerCase(word);

        Node *root = this->root;

        if (root == nullptr) {
            Node *newNode = new Node();
            this->root = newNode;
        }

        for (char c : word) {
            if (root->getChildren(c) == nullptr) {
                Node *newNode = new Node();
                newNode->setParent(root);
                newNode->setLetter(c);
                newNode->setFileNames(fileName);

                root->setChildren(newNode, c);
            }

            root = root->getChildren(c);
        }
    }

    void deleteWord(string word) {

    }

    void search(string word) {

    }
};

bool fillTheTree(Tree &tree, QDir dir, QStringList fileNames);
void textToWords(Tree &tree, QString fileContent, QString fileName);

int main() {
    Tree tree;
    QStringList fileNames;
    QDir dir;

    string folder;
    cin >> folder;

    if (!editFiles(dir, fileNames, folder)) return 0;
    if (!fillTheTree(tree, dir, fileNames)) return 0;
}

//get a list of files' name;
bool editFiles(QDir &dir, QStringList &fileNames, string folder) {
    dir.setPath("C:/Users/bpc/Desktop/" + QString::fromStdString(folder));
    fileNames = dir.entryList(QDir::Files);

    for (QString s : fileNames) {
        QString fileContent;
        openFilesReadOnly(dir, s, fileContent);
        openFilesWriteOnly(dir, s, fileContent);
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
bool openFilesWriteOnly(QDir dir, QString fileName, QString fileContent) {
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

//convert a string to lower case
void toLowerCase(string &word) {
    transform(word.begin(), word.end(), word.begin(), ::tolower);
}

//read every file and extract its text
bool fillTheTree(Tree &tree, QDir dir, QStringList fileNames) {
    for (QString i : fileNames) {
        QFile file(dir.filePath(i));

        if (!file.open(QFile::ReadOnly)) {
            cout << "Cannot open file";
            return false;
        }

        QString fileContent = file.readAll();
        textToWords(tree, fileContent, i);

        file.close();
    }

    return true;
}

//convert file content to words
void textToWords(Tree &tree, QString fileContent, QString fileName) {
    string word = "";

    for (char c : fileContent.toStdString()) {
        if (c == ' ') {
            if (word != "") {
                toLowerCase(word);
                tree.insert(word, fileName);
                word = "";
            }
        } else {
            word += c;
        }
    }
}
