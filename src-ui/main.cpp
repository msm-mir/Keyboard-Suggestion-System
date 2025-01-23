#include "search.h"

#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Search s;
    s.showMaximized();

    return a.exec();
}
