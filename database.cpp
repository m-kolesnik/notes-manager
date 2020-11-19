#include "database.h"

Database::Database(std::string path_to_db)
{
    this->path = path_to_db;
}

Database::~Database()
{

}

bool Database::open()
{
    this->database = QSqlDatabase::addDatabase("QSQLITE");
    this->database.setDatabaseName(QString::fromStdString(this->path));

    if(!this->database.open())
        return false;

    return true;
}

bool Database::close()
{
    this->database.close();
    return true;
}

Note Database::addNote(const Note &note)
{
    QSqlQuery query;

    query.prepare("INSERT INTO notes VALUES (:id, :title, :body, :date, :category, :priority, :status)");
    query.bindValue(":id", note.id);
    query.bindValue(":title", QString::fromStdString(note.title));
    query.bindValue(":body", QString::fromStdString(note.body));
    query.bindValue(":date", QString::fromStdString(note.date));
    query.bindValue(":category", QString::fromStdString(note.category));
    query.bindValue(":priority", note.priority);
    query.bindValue(":status", Status::PENDING);

    if(!query.exec())
        std::cerr << "Couldn't insert note";

    return note;
}

Note Database::getNoteByTitle(const std::string &title)
{
    Note note;
    QSqlQuery query;

    query.prepare("SELECT * FROM notes WHERE title = :title");
    query.bindValue(":title", QString::fromStdString(title));

    query.exec();
    query.first();

    note.id = query.value("id").toUInt();
    note.title = query.value("title").toString().toStdString();
    note.body = query.value("body").toString().toStdString();
    note.date = query.value("date").toString().toStdString();
    note.priority = query.value("priority").toUInt();
    note.status = query.value("status").toUInt();

    return note;
}

Note Database::getNoteById(const unsigned int id)
{
    Note note;
    QSqlQuery query;

    query.prepare("SELECT * FROM notes WHERE id = :id");
    query.bindValue(":title", id);

    query.exec();
    query.first();

    note.id = query.value("id").toUInt();
    note.title = query.value("title").toString().toStdString();
    note.body = query.value("body").toString().toStdString();
    note.date = query.value("date").toString().toStdString();
    note.priority = query.value("priority").toUInt();
    note.status = query.value("status").toUInt();

    return note;
}

std::vector<Note> Database::getAllNotes()
{
    std::vector<Note> notes;
    QSqlQuery query("SELECT * FROM notes");

    while (query.next()) {
        Note note;
        note.id = query.value("id").toUInt();
        note.title = query.value("title").toString().toStdString();
        note.body = query.value("body").toString().toStdString();
        note.date = query.value("date").toString().toStdString();
        note.priority = query.value("priority").toUInt();
        note.status = query.value("status").toUInt();

        notes.push_back(note);
    }

    return notes;
}

Note Database::updateNote(const Note &note)
{
     QSqlQuery query;

     query.prepare("UPDATE notes SET status = :status WHERE id = :id");
     query.bindValue(":id", note.id);
     query.bindValue(":status", note.status);
     query.exec();

     return note;
}

unsigned int Database::deleteNote(unsigned int id)
{
    QSqlQuery query;

    query.prepare("DELETE FROM notes WHERE id = :id");
    query.bindValue(":id", id);

    if(query.exec())
        return id;

    return id;
}

std::vector<Category> Database::getAllCategories()
{
    std::vector<Category> categories;

    QSqlQuery query("SELECT * FROM categories");

    while (query.next()) {
        Category category;
        category.id = query.value("id").toUInt();
        category.name = query.value("name").toString().toStdString();
        categories.push_back(category);
    }

    return categories;
}

std::vector<Note> Database::getNotesByCategory(std::string category)
{
    std::vector<Note> notes;
    QSqlQuery query;

    query.prepare("SELECT * FROM notes WHERE category = :category");
    query.bindValue(":category", QString::fromStdString(category));


    if(query.exec())
    while (query.next()) {
        Note note;
        note.id = query.value("id").toUInt();
        note.title = query.value("title").toString().toStdString();
        note.body = query.value("body").toString().toStdString();
        note.category = query.value("category").toString().toStdString();
        note.date = query.value("date").toString().toStdString();
        note.priority = query.value("priority").toUInt();
        note.status = query.value("status").toUInt();
        notes.push_back(note);
    }
    else {
        std::cerr << query.lastError().text().toStdString();
    }

    return notes;
}
