#include "addnote.h"
#include "ui_addnote.h"
#include <time.h>

addNote::addNote(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addNote)
{
    ui->setupUi(this);
}

addNote::~addNote()
{
    delete ui;
}

void addNote::on_buttonBox_accepted()
{
    this->database = new Database("C:/Users/nikit/Desktop/hometask/notes-manager/data/data.db");
    this->database->database.database(QSqlDatabase::defaultConnection);

    Note note = this->getNote();

    this->database->addNote(note);

    return;
}

Note addNote::getNote(){
    Note note;
    std::vector<Note> notes = this->database->getAllNotes();

    time_t _tm = time(NULL);

    struct tm *currtime = localtime(&_tm);

    note.id = notes[notes.size() - 1].id + 1;
    note.body = this->ui->textEditBody->toPlainText().toStdString();
    note.title = this->ui->lineEditTitle->text().toStdString();
    note.date = asctime(currtime);
    note.category = this->ui->comboBoxCategory->currentText().toStdString();
    note.priority = Priority::DEFAULT;

    if(this->ui->comboBoxPriority->currentText().toStdString() == "low")
        note.priority = Priority::LOW;

    if(this->ui->comboBoxPriority->currentText().toStdString() == "medium")
        note.priority = Priority::MEDIUM;

    if(this->ui->comboBoxPriority->currentText().toStdString() == "high")
        note.priority = Priority::HIGH;

    return note;
}
