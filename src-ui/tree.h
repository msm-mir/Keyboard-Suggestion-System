#ifndef TREE_H
#define TREE_H

#include "node.h"
#include <QDir>

class Tree {
private:
    Node *root;

public:
    Tree();

    void setRoot(Node *root);

    Node* getRoot();

    bool fillTheTree(QDir, QStringList);
    void textToWords(QString, QString);
    void insertWord(string, QString);
    void editInputs(string, string, string, QStringList&, QStringList&, QStringList&);
    QStringList searchFileNames(string, string, string, QDir);
    QStringList searchWord(string);
    void deleteWord(string);
};

#endif // TREE_H
