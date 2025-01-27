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
    QStringList editInputs(QString);
    QStringList searchFileNames(QString, QString, QString, QDir);
    QStringList searchWord(string);
    QStringList fileNamesByCondition(QStringList, QStringList, QStringList, QStringList, QStringList, QDir);
    QStringList findCommonElements(QStringList, QStringList);
    QStringList removeCommonElements(QStringList, QStringList, QStringList, QStringList, QDir);
    void backtrack(QString);
    void deleteWord(string);
};

#endif // TREE_H
