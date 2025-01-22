#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>
#include <QStringList>

using namespace std;

class Node {
private:
    Node *parent;
    vector<Node*> children;
    char letter;
    string word;
    QStringList fileNames;

public:
    Node();

    //setter
    void setParent(Node*);
    void setChildren(Node*, char);
    void setLetter(char);
    void setWord(string);
    void setFileNames(QString);

    //getter
    Node* getParent();
    Node* getChildren(char);
    char getLetter();
    string getWord();
    QStringList getFileNames();

};

#endif // NODE_H
