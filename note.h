#ifndef NOTE_H
#define NOTE_H
#include <string>

struct Note {
    unsigned id;
    std::string title;
    std::string body;
    std::string date;
    std::string category;
    unsigned priority;
    unsigned status;
};

typedef struct Note Note;

#endif // NOTE_H
