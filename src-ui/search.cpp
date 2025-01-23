#include "search.h"
#include "ui_search.h"

Search::Search(QWidget *parent) : QMainWindow(parent), ui(new Ui::Search) {
    ui->setupUi(this);

    connections();
}

Search::~Search() {
    delete ui;
}

void Search::connections() {
    connect(ui->uploadButton, SIGNAL(clicked()), this, SLOT(editFiles()));
    connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(onSearchButtonClicked()));
    connect(ui->filterButton, SIGNAL(toggled()), this, SLOT(onFilterButtonClicked()));
}

void Search::error(QString text, bool set) {

}

//get a list of files' name;
void Search::editFiles() {
    QString folderPath = QFileDialog::getExistingDirectory(this, "Select a Directory", "C:/Users/bpc/Desktop/");
    dir.setPath(folderPath);
    fileNames = dir.entryList(QDir::Files);

    for (QString s : fileNames) {
        QString fileContent;
        openFilesReadOnly(s, fileContent);
        openFilesWriteOnly(s, fileContent);
    }

    if (!tree.fillTheTree(dir, fileNames)) {
        error("Cannot Open Files!", true);
    }
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
    ui->fileNamesLabel->setText("");

    if (ui->mustContainlineEdit->text().isEmpty() &&
        ui->atLeastContainLineEdit->text().isEmpty() &&
        ui->notContainLineEdit->text().isEmpty()) {
        if (ui->searchLineEdit->text().isEmpty()) {
            error("Search Something Bitch!", true);
        } else {
            string search = ui->searchButton->text().toStdString();
            finalFileNames = tree.searchFileNames(search, "", "", dir);
        }
    } else {
        string include = ui->mustContainlineEdit->text().toStdString();
        string atLeastInclude = ui->atLeastContainLineEdit->text().toStdString();
        string notInclude = ui->notContainLineEdit->text().toStdString();

        finalFileNames = tree.searchFileNames(include, atLeastInclude, notInclude, dir);
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

//print file names
void Search::printFileNames(QStringList fileNames) {
    if (fileNames.isEmpty()) {
        error("Not Found!", true);
        return;
    }

    int cnt = 0;
    string output = "";
    for (QString s : fileNames) {
        output += s.toStdString() + "\t";
        if (cnt != 0 && cnt % 5 == 0) output += "\n";
        cnt++;
    }
}
