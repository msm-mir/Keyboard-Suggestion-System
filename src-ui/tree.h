#ifndef TREE_H
#define TREE_H

#include "node.h"

#include <QDir>
#include <QRegularExpression>
#include <QString>
#include <QList>
#include <set>

using namespace std;

class Tree {
private:
    Node *root;

public:
    Tree();

    void setRoot(Node *root);

    Node* getRoot();

    void toLowerCase(string&);
    bool fillTheTree(QDir, QStringList);
    void textToWords(QString, QString);
    void insertWord(string, QString);
    void editInputs(string, string, string, QStringList&, QStringList&, QStringList&);
    QStringList searchFileNames(string, string, string, QDir);
    QStringList searchWord(string);
    //find intersection of two qstringlist
    QStringList fileNamesByCondition(QStringList, QStringList, QStringList, QStringList, QStringList, QDir);
    QStringList findCommonElements(QStringList, QStringList);
    QStringList removeCommonElements(QStringList, QStringList, QStringList, QStringList, QDir);
    void updateSearchingNodes();
    void deleteWord(string);
};

#endif // TREE_H
