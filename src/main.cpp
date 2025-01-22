#include <iostream>
#include <QDir>
#include <QRegularExpression>
#include <set>
#include <vector>

using namespace std;

bool editFiles(QDir&, QStringList&, string);
bool openFilesReadOnly(QDir, QString, QString&);
bool openFilesWriteOnly(QDir, QString, QString);
void toLowerCase(string&);
QStringList fileNamesByCondition(QStringList, QStringList, QStringList, QStringList, QStringList, QDir);
QStringList findCommonElements(QStringList, QStringList);
QStringList removeCommonElements(QStringList, QStringList, QStringList, QStringList, QDir);
void printFileNames(QStringList);

class Node {
private:
    Node *parent;
    vector<Node*> children;
    char letter;
    string word;
    QStringList fileNames;

public:
    Node() {
        this->parent = nullptr;
        this->children.resize(26, nullptr);
        this->letter = 0;
        this->word = "";
        this->fileNames.clear();
    }

    //setter
    void setParent(Node *parent) {
        this->parent = parent;
    }
    void setChildren(Node *child, char idx) {
        this->children.at(idx - 'a') = child;
    }
    void setLetter(char letter) {
        this->letter = letter;
    }
    void setWord(string word) {
        this->word = word;
    }
    void setFileNames(QString fileName) {
        this->fileNames.append(fileName);
    }

    //getter
    Node* getParent() {
        return this->parent;
    }
    Node* getChildren(char idx) {
        return this->children.at(idx - 'a');
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

    //read every file and extract its text
    bool fillTheTree(QDir dir, QStringList fileNames) {
        for (QString i : fileNames) {
            QFile file(dir.filePath(i));

            if (!file.open(QFile::ReadOnly)) {
                cout << "Cannot open file";
                return false;
            }

            QString fileContent = file.readAll();
            this->textToWords(fileContent, i);

            file.close();
        }

        return true;
    }

    //convert file content to words
    void textToWords(QString fileContent, QString fileName) {
        QStringList words = fileContent.split(' ', Qt::SkipEmptyParts);

        for (QString q : words) {
            string s = q.toStdString();
            toLowerCase(s);
            this->insertWord(s, fileName);
        }
    }

    //insert words in tree
    void insertWord(string word, QString fileName) {
        if (this->root == nullptr) {
            Node *newNode = new Node();
            this->root = newNode;
        }

        Node *root = this->root;

        for (char c : word) {
            if (root->getChildren(c) == nullptr) {
                Node *newNode = new Node();
                newNode->setParent(root);
                newNode->setLetter(c);

                root->setChildren(newNode, c);
            }

            root = root->getChildren(c);
        }

        root->setWord(word);
        root->setFileNames(fileName);
    }

    //lower case, remove non-alphabet letters, list words seperate by ','
    void editInputs(string word1, string word2, string word3, QStringList &list1, QStringList &list2, QStringList &list3) {
        toLowerCase(word1);
        toLowerCase(word2);
        toLowerCase(word3);

        QString qWord1 = QString::fromStdString(word1);
        QString qWord2 = QString::fromStdString(word2);
        QString qWord3 = QString::fromStdString(word3);

        qWord1.remove(QRegularExpression("[^a-z,]+"));
        qWord2.remove(QRegularExpression("[^a-z,]+"));
        qWord3.remove(QRegularExpression("[^a-z,]+"));

        list1 = qWord1.split(',', Qt::SkipEmptyParts);
        list2 = qWord2.split(',', Qt::SkipEmptyParts);
        list3 = qWord3.split(',', Qt::SkipEmptyParts);
    }

    //find file names by words
    QStringList searchFileNames(string include, string atLeastInclude, string notInclude, QDir dir) {
        QStringList includeFileNames, atLeastIncludeFileNames, notIncludeFileNames;
        QStringList includeList, atLeastIncludeList, notIncludeList;

        this->editInputs(include, atLeastInclude, notInclude, includeList, atLeastIncludeList, notIncludeList);

        int i = 0;
        for (QString s : includeList) {
            QStringList find = this->searchWord(s.toStdString());
            QStringList common = findCommonElements(includeFileNames, find);

            if (i == 0) {
                includeFileNames.append(find);
            } else {
                includeFileNames = common;
            }

            i++;
        }
        for (QString s : atLeastIncludeList) {
            atLeastIncludeFileNames.append(this->searchWord(s.toStdString()));
        }
        for (QString s : notIncludeList) {
            notIncludeFileNames.append(this->searchWord(s.toStdString()));
        }

        includeFileNames.removeDuplicates();
        atLeastIncludeFileNames.removeDuplicates();
        notIncludeFileNames.removeDuplicates();

        return fileNamesByCondition(includeList, atLeastIncludeList, includeFileNames, atLeastIncludeFileNames, notIncludeFileNames, dir);
    }

    //search words in tree and return file names
    QStringList searchWord(string word) {
        if (word == "") {
            return QStringList();
        }

        Node *root = this->root;

        for (char c : word) {
            if (root->getChildren(c) == nullptr) {
                return QStringList();
            }
            root = root->getChildren(c);

        }

        if (root->getWord() == word) {
            return root->getFileNames();
        }

        return QStringList();
    }

    void updateSearchingNodes() {

    }

    //delete a word from the tree
    void deleteWord(string word) {
        if (this->root == nullptr || word == "") {
            return;
        }

        Node *root = this->root;

        for (char c : word) {
            if (root->getChildren(c) == nullptr) {
                return;
            }
            root = root->getChildren(c);
        }

        if (root->getWord() == word) {
            root->setWord("");
        }
    }
};

int main() {
    Tree tree;
    QStringList fileNames;
    QDir dir;

    cout << "Folder: ";
    string folder;
    getline(cin, folder);

    if (!editFiles(dir, fileNames, folder)) return 0;
    if (!tree.fillTheTree(dir, fileNames)) return 0;

    while (true) {
        string include, atLeastInclude, notInclude;

        cout << "Include: ";
        getline(cin, include);

        cout << "At Least Include: ";
        getline(cin, atLeastInclude);

        cout << "Not Include: ";
        getline(cin, notInclude);

        QStringList finalFileNames = tree.searchFileNames(include, atLeastInclude, notInclude, dir);
        printFileNames(finalFileNames);
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

//find the right file names for output
QStringList fileNamesByCondition(QStringList includeList, QStringList atLeastIncludeList, QStringList include, QStringList atLeastInclude, QStringList notInclude, QDir dir) {
    QStringList common = findCommonElements(include, atLeastInclude);

    if (common.isEmpty()) {
        if (include.isEmpty() && includeList.isEmpty()) {
            common = atLeastInclude;
        }
        if (atLeastInclude.isEmpty()) {
            common = include;
        }
    }

    return removeCommonElements(includeList, atLeastIncludeList, common, notInclude, dir);
}

//find intersection of two qstringlist
QStringList findCommonElements(QStringList list1, QStringList list2) {
    set<QString> set1(list1.begin(), list1.end());
    set<QString> set2(list2.begin(), list2.end());

    set<QString> intersectionSet;
    set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(intersectionSet, intersectionSet.begin()));

    return QStringList(intersectionSet.begin(), intersectionSet.end());
}

//remove intersection of two qstringlist
QStringList removeCommonElements(QStringList primary1, QStringList primary2, QStringList list1, QStringList list2, QDir dir) {
    if (primary1.isEmpty() && primary2.isEmpty()) {
        list1 = dir.entryList(QDir::Files);
    }

    set<QString> set1(list1.begin(), list1.end());
    set<QString> set2(list2.begin(), list2.end());

    set<QString> difference;
    set_difference(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(difference, difference.begin()));

    return QStringList(difference.begin(), difference.end());
}

//print file names
void printFileNames(QStringList fileNames) {
    if (fileNames.isEmpty()) {
        cout << "Not Found!\n\n";
        return;
    }

    int cnt = 0;
    for (QString s : fileNames) {
        cout << s.toStdString() << "\t";
        if (cnt != 0 && cnt % 5 == 0) cout << "\n";
        cnt++;
    }
    cout << "\n\n";
}
