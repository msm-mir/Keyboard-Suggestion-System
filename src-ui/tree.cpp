#include "tree.h"

Tree::Tree() {
    this->root = nullptr;
}

//setter
void Tree::setRoot(Node *root) {
    this->root = root;
}

//getter
Node* Tree::getRoot() {
    return this->root;
}

//convert a string to lower case
void Tree::toLowerCase(string &word) {
    transform(word.begin(), word.end(), word.begin(), ::tolower);
}

//read every file and extract its text
bool Tree::fillTheTree(QDir dir, QStringList fileNames) {
    for (QString i : fileNames) {
        QFile file(dir.filePath(i));

        if (!file.open(QFile::ReadOnly)) {
            return false;
        }

        QString fileContent = file.readAll();
        textToWords(fileContent, i);

        file.close();
    }

    return true;
}

//convert file content to words
void Tree::textToWords(QString fileContent, QString fileName) {
    QStringList words = fileContent.split(' ', Qt::SkipEmptyParts);

    for (QString q : words) {
        string s = q.toStdString();
        toLowerCase(s);
        insertWord(s, fileName);
    }
}

//insert words in tree
void Tree::insertWord(string word, QString fileName) {
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

//find file names by words
QStringList Tree::searchFileNames(string include, string atLeastInclude, string notInclude, QDir dir) {
    QStringList includeFileNames, atLeastIncludeFileNames, notIncludeFileNames;
    QStringList includeList, atLeastIncludeList, notIncludeList;

    editInputs(include, atLeastInclude, notInclude, includeList, atLeastIncludeList, notIncludeList);

    int i = 0;
    for (QString s : includeList) {
        QStringList find = searchWord(s.toStdString());
        QStringList common = findCommonElements(includeFileNames, find);

        if (i == 0) {
            includeFileNames.append(find);
        } else {
            includeFileNames = common;
        }

        i++;
    }
    for (QString s : atLeastIncludeList) {
        atLeastIncludeFileNames.append(searchWord(s.toStdString()));
    }
    for (QString s : notIncludeList) {
        notIncludeFileNames.append(searchWord(s.toStdString()));
    }

    includeFileNames.removeDuplicates();
    atLeastIncludeFileNames.removeDuplicates();
    notIncludeFileNames.removeDuplicates();

    return fileNamesByCondition(includeList, atLeastIncludeList, includeFileNames, atLeastIncludeFileNames, notIncludeFileNames, dir);
}

//lower case, remove non-alphabet letters, list words seperate by ','
void Tree::editInputs(string word1, string word2, string word3, QStringList &list1, QStringList &list2, QStringList &list3) {
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

//search words in tree and return file names
QStringList Tree::searchWord(string word) {
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

//find intersection of two qstringlist
QStringList Tree::findCommonElements(QStringList list1, QStringList list2) {
    set<QString> set1(list1.begin(), list1.end());
    set<QString> set2(list2.begin(), list2.end());

    set<QString> intersectionSet;
    set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(intersectionSet, intersectionSet.begin()));

    return QStringList(intersectionSet.begin(), intersectionSet.end());
}

//find the right file names for output
QStringList Tree::fileNamesByCondition(QStringList includeList, QStringList atLeastIncludeList, QStringList include, QStringList atLeastInclude, QStringList notInclude, QDir dir) {
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

void Tree::updateSearchingNodes() {

}

//delete a word from the tree
void Tree::deleteWord(string word) {
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
        root->setFileNames("");
    }
}
