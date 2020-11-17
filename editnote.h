#ifndef EDITNOTE_H
#define EDITNOTE_H

#include <QDialog>
#include "note.h"
#include "database.h"

namespace Ui {
class editNote;
}

class editNote : public QDialog
{
    Q_OBJECT

public:
    explicit editNote(QWidget *parent = nullptr);
    ~editNote();

private:
    Ui::editNote *ui;
    Note baseNote;
};

#endif // EDITNOTE_H
