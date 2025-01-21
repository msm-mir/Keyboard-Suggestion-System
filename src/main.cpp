#include <iostream>
#include <QDir>
#include <QRegularExpression>

using namespace std;

bool editFiles(QDir&, QStringList&, string);
bool openFilesReadOnly(QDir, QString, QString&);
bool openFilesWriteOnly(QDir, QString, QString);
void toLowerCase(string&);
QStringList conditionFileNames(QStringList, QStringList, QStringList);

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

    //insert words in tree
    void insertWord(string word, QString fileName) {
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

    //delete a word in tree
    void deleteWord(string word) {

    }

    //find file names by words
    QStringList searchFileNames(QString qInclude, QString qAtLeastInclude, QString qNotInclude) {
        QStringList includeList = qInclude.split(' ', Qt::SkipEmptyParts);
        QStringList atLeastIncludeList = qAtLeastInclude.split(' ', Qt::SkipEmptyParts);
        QStringList notIncludeList = qNotInclude.split(' ', Qt::SkipEmptyParts);

        QStringList includeFileNames, atLeastIncludeFileNames, notIncludeFileNames;

        for (QString s : includeList) {
            includeFileNames.append(this->searchWord(s));
        }
        for (QString s : atLeastIncludeList) {
            atLeastIncludeFileNames.append(this->searchWord(s));
        }
        for (QString s : notIncludeList) {
            notIncludeFileNames.append(this->searchWord(s));
        }

        return conditionFileNames(includeFileNames, atLeastIncludeFileNames, notIncludeFileNames);
    }

    //search words in tree and return file names
    QStringList searchWord(QString word) {
        if (word.isEmpty()) return QStringList();


    }
};

bool fillTheTree(Tree&, QDir, QStringList);
void textToWords(Tree&, QString, QString);

int main() {
    Tree tree;
    QStringList fileNames;
    QDir dir;

    cout << "Folder: ";
    string folder;
    cin >> folder;

    if (!editFiles(dir, fileNames, folder)) return 0;
    if (!fillTheTree(tree, dir, fileNames)) return 0;

    while (true) {
        string include, atLeastInclude, notInclude;

        cout << "Include: ";
        cin >> include;
        toLowerCase(include);
        QString qInclude = QString::fromStdString(include);

        cout << "At Least Include: ";
        cin >> atLeastInclude;
        toLowerCase(atLeastInclude);
        QString qAtLeastInclude = QString::fromStdString(atLeastInclude);

        cout << "Not Include: ";
        cin >> notInclude;
        toLowerCase(notInclude);
        QString qNotInclude = QString::fromStdString(notInclude);

        if (tree.searchFileNames(qInclude, qAtLeastInclude, qNotInclude).isEmpty()) {
            cout << "Not Found" << endl;
        } else {

        }
    }
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
    QStringList words = fileContent.split(' ', Qt::SkipEmptyParts);

    for (QString q : words) {
        string s = q.toStdString();
        toLowerCase(s);
        tree.insertWord(s, fileName);
    }
}

//find the right file names for output
QStringList conditionFileNames(QStringList include, QStringList atLeastInclude, QStringList notInclude) {

}
