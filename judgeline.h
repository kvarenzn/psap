#ifndef JUDGELINE_H
#define JUDGELINE_H

#include <QVector>
#include <QJsonObject>
#include <QPair>
#include "note.h"
#include "events.h"

class JudgeLine
{
public:

    QVector<Note> notesAbove;
    QVector<Note> notesBelow;
    double bpm;
    QVector<SpeedEvent> speedEvents;
    QVector<NormalEvent> disappearEvents;
    QVector<NormalEvent> moveEvents;
    QVector<NormalEvent> rotateEvents;

    JudgeLine();

    static JudgeLine fromJsonObject(const QJsonObject &data);
    static JudgeLine fromJsonObjectV1(const QJsonObject &data);

    double floor(double time) const;
    double secondsToTime(double seconds) const;
    double timeToSeconds(double time) const;
    double opacity(double time) const;
    QPair<double, double> position(double time) const;
    double angle(double time) const;
    QPair<double, double> getNotePosition(const Note &note, double time) const;
    QPair<double, double> getNotePosition(const Note &note) const;

    void markDualNotes();
};

#endif // JUDGELINE_H
