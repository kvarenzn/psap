#ifndef CHART_H
#define CHART_H

#include <QList>
#include <QHash>
#include <QJsonObject>

#include "judgeline.h"

struct SingleNoteInfo {
    double posX;
    double posY;
    double alpha;
    int lasts;
    Note *note;
};

struct HoldNoteInfo {
    int begin;
    double posX;
    double posY;
    double alpha;
    Note *note;
};

class Chart
{
public:
    Chart();

    int version;
    int numOfNotes;
    double offset;
    QList<JudgeLine> judgeLines;

    QMap<int, QList<SingleNoteInfo>> singleNotesByCs;
    QMap<int, QList<HoldNoteInfo>> holdsByCs;

    static Chart fromJsonObject(const QJsonObject &data);
    static Chart loadFile(const QString &filepath);

    void markDualNotes();
    void preprocess();

    int getNextKeyFrame(double seconds);
    int getPrevKeyFrame(double seconds);
};

#endif // CHART_H
