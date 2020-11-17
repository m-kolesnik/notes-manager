#include "editnote.h"
#include "ui_editnote.h"

editNote::editNote(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editNote)
{
    ui->setupUi(this);
}

editNote::~editNote()
{
    delete ui;
}
