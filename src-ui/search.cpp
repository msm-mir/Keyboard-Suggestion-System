#include "search.h"
#include "ui_search.h"
#include <QPushButton>
#include <QMainWindow>
#include <QFileDialog>

Search::Search(QWidget *parent) : QMainWindow(parent), ui(new Ui::Search) {
    ui->setupUi(this);
}

Search::~Search() {
    delete ui;
}
