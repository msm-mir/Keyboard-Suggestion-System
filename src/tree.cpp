#include "tree.h"
#include <set>

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
QStringList Tree::searchFileNames(QString include, QString atLeastInclude, QString notInclude, QDir dir) {
    QStringList includeFileNames, atLeastIncludeFileNames, notIncludeFileNames;

    QStringList includeList = editInputs(include);
    QStringList atLeastIncludeList = editInputs(atLeastInclude);
    QStringList notIncludeList = editInputs(notInclude);

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
QStringList Tree::editInputs(QString word) {
    string s = word.toStdString();

    toLowerCase(s);

    word = QString::fromStdString(s);

    word.remove(QRegularExpression("[^a-z,]+"));

    return word.split(',', Qt::SkipEmptyParts);
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

//find intersection of two qstringlist
QStringList Tree::findCommonElements(QStringList list1, QStringList list2) {
    set<QString> set1(list1.begin(), list1.end());
    set<QString> set2(list2.begin(), list2.end());

    set<QString> intersectionSet;
    set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(intersectionSet, intersectionSet.begin()));

    return QStringList(intersectionSet.begin(), intersectionSet.end());
}

//remove intersection of two qstringlist
QStringList Tree::removeCommonElements(QStringList primary1, QStringList primary2, QStringList list1, QStringList list2, QDir dir) {
    if (primary1.isEmpty() && primary2.isEmpty()) {
        list1 = dir.entryList(QDir::Files);
    }

    set<QString> set1(list1.begin(), list1.end());
    set<QString> set2(list2.begin(), list2.end());

    set<QString> difference;
    set_difference(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(difference, difference.begin()));

    return QStringList(difference.begin(), difference.end());
}

//search close words
QStringList Tree::suggestWords(QString searchedWord) {
    QStringList words = editInputs(searchedWord);

    if (words.size() == 1) {
        QStringList similarWords;

        increaseLetter(similarWords, searchedWord.toStdString());
        decreaseLetter(similarWords, searchedWord.toStdString());
        changeLetter(similarWords, searchedWord.toStdString());

        similarWords.removeDuplicates();

        return similarWords;
    }

    return QStringList();
}

//add a letter to the word and search it
void Tree::increaseLetter(QStringList &list, string word) {
    for (int i = 0; i <= (int)word.length(); i++) {
        for (int j = 0; j < 26; j++) {
            Node *root = this->root;
            bool check = true;

            for (int k = 0; k < i; k++) {
                if (root->getChildren(word[k]) == nullptr) {
                    check = false;
                    break;
                }
                root = root->getChildren(word[k]);
            }
            if (!check) continue;

            if (root->getChildren(j + 'a') == nullptr) {
                continue;
            }
            root = root->getChildren(j + 'a');

            for (int k = i; k < (int)word.length(); k++) {
                if (root->getChildren(word[k]) == nullptr) {
                    check = false;
                    break;
                }
                root = root->getChildren(word[k]);
            }

            if (check && root->getWord() != "") {
                list.append(QString::fromStdString(root->getWord()));
            }
        }
    }

}

void Tree::decreaseLetter(QStringList &list, string word) {
    for (int i = 0; i < (int)word.length(); i++) {
        Node *root = this->root;
        bool check = true;

        for (int k = 0; k < i; k++) {
            if (root->getChildren(word[k]) == nullptr) {
                check = false;
                break;
            }
            root = root->getChildren(word[k]);
        }
        if (!check) continue;

        for (int k = i + 1; k < (int)word.length(); k++) {
            if (root->getChildren(word[k]) == nullptr) {
                check = false;
                break;
            }
            root = root->getChildren(word[k]);
        }

        if (check && root->getWord() != "") {
            list.append(QString::fromStdString(root->getWord()));
        }
    }
}

void Tree::changeLetter(QStringList &list, string word) {
    for (int i = 0; i < (int)word.length(); i++) {
        for (int j = 0; j < 26; j++) {
            Node *root = this->root;
            bool check = true;

            for (int k = 0; k < i; k++) {
                if (root->getChildren(word[k]) == nullptr) {
                    check = false;
                    break;
                }
                root = root->getChildren(word[k]);
            }
            if (!check) continue;

            if (root->getChildren(j + 'a') == nullptr) {
                continue;
            }
            root = root->getChildren(j + 'a');

            for (int k = i + 1; k < (int)word.length(); k++) {
                if (root->getChildren(word[k]) == nullptr) {
                    check = false;
                    break;
                }
                root = root->getChildren(word[k]);
            }

            if (check && root->getWord() != "") {
                list.append(QString::fromStdString(root->getWord()));
            }
        }
    }
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
