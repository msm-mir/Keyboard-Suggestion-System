#include "node.h"

Node::Node() {
    this->parent = nullptr;
    this->children.resize(26, nullptr);
    this->letter = 0;
    this->word = "";
    this->fileNames.clear();
}

//setter
void Node::setParent(Node *parent) {
    this->parent = parent;
}
void Node::setChildren(Node *child, char idx) {
    this->children.at(idx - 'a') = child;
}
void Node::setLetter(char letter) {
    this->letter = letter;
}
void Node::setWord(string word) {
    this->word = word;
}
void Node::setFileNames(QString fileName) {
    this->fileNames.append(fileName);
}

//getter
Node* Node::getParent() {
    return this->parent;
}
Node* Node::getChildren(char idx) {
    return this->children.at(idx - 'a');
}
char Node::getLetter() {
    return this->letter;
}
string Node::getWord() {
    return this->word;
}
QStringList Node::getFileNames() {
    return this->fileNames;
}
