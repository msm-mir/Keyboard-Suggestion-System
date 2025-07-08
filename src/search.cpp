#include "search.h"
#include "ui_search.h"

Search::Search(QWidget *parent) : QMainWindow(parent), ui(new Ui::Search) {
    ui->setupUi(this);

    ui->listWidget->hide();
    progress(0, false);

    connections();

    error("", false);
    onFilterButtonClicked();
}

Search::~Search() {
    delete ui;
}

void Search::connections() {
    //click on buttons
    connect(ui->uploadButton, SIGNAL(clicked()), this, SLOT(editFiles()));
    connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(onSearchButtonClicked()));
    connect(ui->filterButton, SIGNAL(clicked()), this, SLOT(onFilterButtonClicked()));
    connect(ui->listWidget, &QListWidget::itemClicked, this, &Search::onItemClicked);

    //click enter to search
    connect(ui->searchLineEdit, SIGNAL(returnPressed()), ui->searchButton, SLOT(click()));
    connect(ui->mustContainlineEdit, SIGNAL(returnPressed()), ui->searchButton, SLOT(click()));
    connect(ui->atLeastContainLineEdit, SIGNAL(returnPressed()), ui->searchButton, SLOT(click()));
    connect(ui->notContainLineEdit, SIGNAL(returnPressed()), ui->searchButton, SLOT(click()));

    //set cursor
    ui->searchLineEdit->setFocus();
}

void Search::error(QString text, bool set) {
    if (set) {
        ui->errorLabel->setText(text);
        ui->errorLabel->show();
    } else {
        ui->errorLabel->hide();
    }
}

void Search::progress(int prog, bool set) {
    ui->progressBar->setFixedHeight(10);
    if (set) {
        ui->progressBar->setValue((prog * 100) / fileNames.size());
        ui->progressBar->show();
    } else {
        ui->progressBar->hide();
        ui->progressBar->setMinimum(0);
        ui->progressBar->setMaximum(100);
        ui->progressBar->setValue(0);
    }
}

//get a list of files' name;
void Search::editFiles() {
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select a Directory", "C:/Users/bpc/Desktop/");

    dir.setPath(folderPath);
    fileNames = dir.entryList(QDir::Files);

    progress(0, true);
    int prog = 0;
    for (QString s : fileNames) {
        QString fileContent;
        openFilesReadOnly(s, fileContent);
        openFilesWriteOnly(s, fileContent);

        prog++;
        progress(prog, true);
        QApplication::processEvents();
    }

    if (!tree.fillTheTree(dir, fileNames)) {
        error("Cannot Open Files!", true);
    }

    progress(0, false);
}

//open file to read and edit its text
void Search::openFilesReadOnly(QString fileName, QString &fileContent) {
    QFile file(dir.filePath(fileName));

    if (!file.open(QFile::ReadOnly)) {
        error("Cannot Open Files!", true);
        return;
    }

    fileContent = file.readAll();
    fileContent.remove(QRegularExpression("[^a-zA-Z ]+"));

    file.close();
}

//open file to write the edited text into it
void Search::openFilesWriteOnly(QString fileName, QString fileContent) {
    QFile file(dir.filePath(fileName));

    if (!file.open(QFile::WriteOnly)) {
        error("Cannot Open Files For Writing!", true);
        return;
    }

    QTextStream out(&file);
    out << fileContent;

    file.close();
}

void Search::onSearchButtonClicked() {
    ui->tableWidget->hide();
    ui->listWidget->hide();
    error("", false);

    if (!dir.absolutePath().contains("C:/Users/bpc/Desktop/")) {
        error("Select A Folder!", true);
        return;
    }

    if (ui->filterButton->isChecked()) {
        if (ui->mustContainlineEdit->text().isEmpty() && ui->atLeastContainLineEdit->text().isEmpty() && ui->notContainLineEdit->text().isEmpty()) {
            if (ui->searchLineEdit->text().isEmpty()) {
                error("Search Something!", true);
                return;
            } else {
                finalFileNames = tree.searchFileNames(ui->searchLineEdit->text(), "", "", dir);
            }
        } else {
            finalFileNames = tree.searchFileNames(ui->mustContainlineEdit->text(), ui->atLeastContainLineEdit->text(), ui->notContainLineEdit->text(), dir);
        }
    } else {
        if (ui->searchLineEdit->text().isEmpty()) {
            error("Search Something!", true);
            return;
        }

        finalFileNames = tree.searchFileNames(ui->searchLineEdit->text(), "", "", dir);

        if (finalFileNames.isEmpty()) {
            createList();
        }
    }

    printFileNames(finalFileNames);
}

void Search::onFilterButtonClicked() {
    if (ui->filterButton->isChecked()) {
        ui->mustContainlineEdit->show();
        ui->atLeastContainLineEdit->show();
        ui->notContainLineEdit->show();
    } else {
        ui->mustContainlineEdit->hide();
        ui->atLeastContainLineEdit->hide();
        ui->notContainLineEdit->hide();

        ui->mustContainlineEdit->setText("");
        ui->atLeastContainLineEdit->setText("");
        ui->notContainLineEdit->setText("");
    }
}

//convert a string to lower case
void Search::toLowerCase(string &word) {
    transform(word.begin(), word.end(), word.begin(), ::tolower);
}

//find intersection of two qstringlist
QStringList Search::findCommonElements(QStringList list1, QStringList list2) {
    set<QString> set1(list1.begin(), list1.end());
    set<QString> set2(list2.begin(), list2.end());

    set<QString> intersectionSet;
    set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(intersectionSet, intersectionSet.begin()));

    return QStringList(intersectionSet.begin(), intersectionSet.end());
}

void Search::createList() {
    QStringList list = tree.suggestWords(ui->searchLineEdit->text());

    ui->listWidget->clear();

    for (QString q : list) {
        ui->listWidget->addItem(q);
    }

    ui->listWidget->setFixedHeight(251);
    ui->listWidget->show();
}

void Search::onItemClicked(QListWidgetItem *item) {
    ui->searchLineEdit->setText(item->text());
    ui->listWidget->hide();

    onSearchButtonClicked();
}

//print file names
void Search::printFileNames(QStringList fileNames) {
    if (fileNames.isEmpty()) {
        error("Not Found!", true);
        return;
    }

    FileNamesTable table(ui->tableWidget, fileNames, dir.absolutePath());

    int totalWidth = 0;
    for (int i = 0; i < ui->tableWidget->columnCount(); ++i) {
        totalWidth += ui->tableWidget->columnWidth(i);
    }
    totalWidth += ui->tableWidget->verticalHeader()->width();

    ui->tableWidget->setFixedSize(totalWidth + 17, 411);

    int x = (1920 - ui->tableWidget->width()) / 2;
    ui->tableWidget->move(x - 185, 310);
    ui->tableWidget->show();
}
