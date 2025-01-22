#ifndef SEARCH_H
#define SEARCH_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Search; }
QT_END_NAMESPACE

class Search : public QMainWindow {
    Q_OBJECT

public:
    Search(QWidget *parent = nullptr);
    ~Search();

    void connections();

private slots:
    void onBrowseButtonClicked();

private:
    Ui::Search *ui;
};
#endif // SEARCH_H
