#include "search.h"
#include "ui_search.h"
#include <QPushButton>
#include <QMainWindow>
#include <QFileDialog>

Search::Search(QWidget *parent) : QMainWindow(parent), ui(new Ui::Search) {
    ui->setupUi(this);

    connections();
}

Search::~Search() {
    delete ui;
}

void Search::connections() {
    connect(ui->uploadButton, SIGNAL(clicked()), this, SLOT(onBrowseButtonClicked()));

}

void Search::onBrowseButtonClicked() {
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select a Directory", "C:/Users/bpc/Desktop/");
}
