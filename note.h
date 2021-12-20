#ifndef NOTE_H
#define NOTE_H

#include <QJsonObject>

enum NoteType {
    TAP = 1,
    DRAG,
    HOLD,
    FLICK
};

class Note
{
public:

    NoteType type;
    int timestamp;
    double x;
    double hold;
    double speed;
    double floor;
    bool dual = false;

    Note(NoteType type, int timestamp, double x, double hold, double speed, double floor);

    static Note fromJsonObject(const QJsonObject &data);
};

#endif // NOTE_H
