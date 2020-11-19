#include "mainwindow.h"
#include "ui_mainwindow.h"

std::string getPriorityText(const Note & note);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->icon = new QSystemTrayIcon(this);
    this->icon->setIcon(this->style()->standardIcon(QStyle::SP_ComputerIcon));
    this->icon->show();

    this->database = new Database("C:/Users/nikit/Desktop/hometask/notes-manager/data/data.db");

    if(this->database->open()) {
        QMessageBox::information(0, "Success", "Database is loaded");
    }
    else {
        QMessageBox::critical(0, "Error", "Couldn't open database");
    }

    this->ui->listView->setViewMode(QListView::ViewMode::ListMode);
    this->ui->listView->setFlow(QListView::Flow::TopToBottom);
    this->ui->plainTextEdit->setReadOnly(true);
    this->ui->lineEdit->setReadOnly(true);

    this->updateListView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateListView()
{
    QStringListModel* model = new QStringListModel(this);
    QStringList list;

    std::vector<Note> notes = this->database->getAllNotes();

    std::cerr << "Updating list view, loaded notes: " << notes.size() << std::endl;

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
    dialog->exec();

    this->updateListView();
}

void MainWindow::on_listView_pressed(const QModelIndex &index)
{
    std::string title = this->ui->listView->currentIndex().data().toString().toStdString();

    Note note = this->database->getNoteByTitle(title);

    this->ui->lineEdit->setText(QString::fromStdString(note.title));
    this->ui->plainTextEdit->setPlainText(QString::fromStdString(note.body));
    this->ui->labelDateTime->setText(QString::fromStdString(note.date));
    this->ui->labelPriority->setText(QString::fromStdString(getPriorityText(note)));

    if(note.status == 2)
        this->ui->pushButtonCompleteNote->setDisabled(true);

    if(note.status == 1)
        this->ui->pushButtonCompleteNote->setDisabled(false);

    this->currentNote = note;
}

void MainWindow::on_pushButtonRemoveNote_clicked()
{
    if(this->currentNote.id){
        std::string text = "Are you sure want to remove " + this->currentNote.title;
        int choice = QMessageBox::warning(0, "Warning", QString::fromStdString(text), QMessageBox::StandardButton::Yes, QMessageBox::StandardButton::No);

        switch (choice) {
        case QMessageBox::StandardButton::Yes:
            this->database->deleteNote(this->currentNote.id);
            this->updateListView();

            this->ui->lineEdit->setText("");
            this->ui->plainTextEdit->setPlainText("");

            break;

        case QMessageBox::StandardButton::No:
            break;
        }
    } else {
        QMessageBox::warning(0, "Note is not specified", "Select the note first, then click on the remove button");
    }
}

std::string getPriorityText(const Note & note){
    std::string priority;

    switch (note.priority) {
    case Priority::DEFAULT:
        priority = "";
        break;

    case Priority::LOW:
        priority = "low priority";
        break;

    case Priority::MEDIUM:
        priority = "medium priority";
        break;

    case Priority::HIGH:
        priority = "high priority";
        break;

    default:
        priority = "";
    }

    return priority;
}

void MainWindow::filterByDate()
{
    this->updateListView();
}

void MainWindow::filterByPriority()
{
    QStringListModel* model = new QStringListModel(this);
    QStringList list;

    std::vector<Note> notes = this->database->getAllNotes();

    std::vector<Note> high;
    std::vector<Note> medium;
    std::vector<Note> low;

    for (size_t i = 0; i < notes.size(); i++) {
        if(notes[i].priority == Priority::HIGH)
            high.push_back(notes[i]);

        if(notes[i].priority == Priority::MEDIUM)
            medium.push_back(notes[i]);

        if(notes[i].priority == Priority::LOW)
            low.push_back(notes[i]);
    }

    for (size_t i = 0; i < high.size(); i++) {
        list.push_back(QString::fromStdString(high[i].title));
    }

    for (size_t i = 0; i < medium.size(); i++) {
        list.push_back(QString::fromStdString(medium[i].title));
    }

    for (size_t i = 0; i < low.size(); i++) {
        list.push_back(QString::fromStdString(low[i].title));
    }

    model->setStringList(list);

    this->ui->listView->setModel(model);
}

void MainWindow::on_pushButton_2_clicked()
{
    this->filterByDate();
}

void MainWindow::on_pushButton_3_clicked()
{
    this->filterByPriority();
}

void MainWindow::on_pushButtonEditNote_clicked()
{
    QDialog *dialog = new addNote(this);

    unsigned id = this->database->deleteNote(this->currentNote.id);

    dialog->setModal(true);
    dialog->exec();

    this->updateListView();

    Note note = this->database->getNoteById(id);

    this->ui->lineEdit->setText(QString::fromStdString(note.title));
    this->ui->plainTextEdit->setPlainText(QString::fromStdString(note.body));
    this->ui->labelDateTime->setText(QString::fromStdString(note.date));
    this->ui->labelPriority->setText(QString::fromStdString(getPriorityText(note)));

    if(note.status == 2)
        this->ui->lineEdit->setStyleSheet("text-decoration: line-through;");

    if(note.status == 1)
        this->ui->lineEdit->setStyleSheet("");

    this->currentNote = note;
}

void MainWindow::on_comboBoxCategory_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "all")
       return this->updateListView();

    if(arg1 == "personal") {
        std::vector<Note> personalNotes = this->database->getNotesByCategory("personal");
        QStringListModel* model = new QStringListModel(this);
        QStringList list;

        std::cerr << "Updating list view: " << personalNotes.size() << " found" << std::endl;

        for (size_t i = 0; i < personalNotes.size(); i++) {
            list.push_back(QString::fromStdString(personalNotes[i].title));
        }

        model->setStringList(list);
        this->ui->listView->setModel(model);

    }

    if(arg1 == "work") {
        std::vector<Note> workNotes = this->database->getNotesByCategory("work");

        QStringListModel* model = new QStringListModel(this);
        QStringList list;

        std::cerr << "Updating list view: " << workNotes.size() << " found" << std::endl;

        for (size_t i = 0; i < workNotes.size(); i++) {
            list.push_back(QString::fromStdString(workNotes[i].title));
        }

        model->setStringList(list);
        this->ui->listView->setModel(model);
    }
}

void MainWindow::on_pushButtonCompleteNote_clicked()
{
    if(this->currentNote.id){
        Note note = this->database->getNoteByTitle(this->currentNote.title);
        note.status = Status::RESOLVED;

        this->database->updateNote(note);

        this->ui->pushButtonCompleteNote->setDisabled(true);
        this->currentNote = note;
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    std::ofstream data_file;
    std::vector<Note> notes = this->database->getAllNotes();

    data_file.open("../notes-manager/data/notes_data.txt");

    for (size_t i = 0; i < notes.size(); i++) {
        if(notes[i].id != 0) {
            data_file << i << ". " << notes[i].title << " | " << notes[i].body << "|" << notes[i].category << std::endl;
        }
    }

    data_file.close();
    QMessageBox::information(0, "Info", "Successfuly exported to file");
}
