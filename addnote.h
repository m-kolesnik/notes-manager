#ifndef ADDNOTE_H
#define ADDNOTE_H

#include "note.h"
#include "database.h"
#include <QDialog>

namespace Ui {
class addNote;
}

class addNote : public QDialog
{
    Q_OBJECT

public:
    explicit addNote(QWidget *parent = nullptr);
    ~addNote();

    Note getNote();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::addNote *ui;
    Database *database;
};

#endif // ADDNOTE_H
