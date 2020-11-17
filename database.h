#ifndef DATABASE_H
#define DATABASE_H
#include <iostream>
#include <string>
#include <vector>
#include <QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>

#include "note.h"
#include "status.h"
#include "category.h"

class Database
{
public:
    Database(std::string path_to_db);
    ~Database();

    bool open();
    bool close();

    Note addNote(const Note & note);
    Note updateNote(const Note & note);
    Note getNoteById(const unsigned id);
    Note getNoteByTitle(const std::string & title);

    unsigned int deleteNote(unsigned id);

    std::vector<Note> getAllNotes();

    std::vector<Category> getAllCategories();
    std::vector<Note> getNotesByCategory(std::string category);

    QSqlDatabase database;

private:
    std::string path;
};

#endif // DATABASE_H
