#include "search.h"
#include "tree.h"

#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Tree tree;

    Search s;
    s.show();

    return a.exec();
}
