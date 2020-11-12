#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->database = new Database("C:/Users/nikit/Desktop/hometask/notes-manager/data/data.db");

    if(this->database->open()) {
        QMessageBox::information(0, "Success", "Database is loaded");
    }
    else {
        QMessageBox::critical(0, "Error", "Couldn't open database");
    }

    this->ui->listView->setViewMode(QListView::ViewMode::ListMode);
    this->ui->listView->setFlow(QListView::Flow::TopToBottom);
    this->updateListView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateListView(){
    QStringListModel* model = new QStringListModel(this);
    QStringList list;

    std::vector<Note> notes = this->database->getAllNotes();

    for (size_t i = 0; i < notes.size(); i++) {
        list.push_back(QString::fromStdString(notes[i].title));
    }

    model->setStringList(list);

    this->ui->listView->setModel(model);
}


void MainWindow::on_pushButton_clicked()
{
    QDialog *dialog = new addNote(this);

    dialog->setModal(true);
    dialog->show();
}
