#include "note.h"

Note::Note(NoteType type, int timestamp, double x, double hold, double speed, double floor)
{
    this->type = type;
    this->timestamp = timestamp;
    this->x = x;
    this->hold = hold;
    this->speed = speed;
    this->floor = floor;
}

Note Note::fromJsonObject(const QJsonObject &data) {
    return Note(
            NoteType(data["type"].toInt()),
            data["time"].toInt(),
            data["positionX"].toDouble(),
            data["holdTime"].toDouble(),
            data["speed"].toDouble(),
            data["floorPosition"].toDouble());
}
