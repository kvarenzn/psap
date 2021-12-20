#ifndef FRAMEMANAGER_H
#define FRAMEMANAGER_H

#include <QObject>

#include "linesmodel.h"
#include "notesmodel.h"
#include "holdsmodel.h"

#include "chart.h"

class FrameManager : public QObject
{
    Q_OBJECT
public:
    explicit FrameManager(QObject *parent = nullptr);

    void bindModels(LinesModel *judgeLines, NotesModel *taps, NotesModel *drags, NotesModel *flicks, HoldsModel *holds);
    void clear();

    void loadChart(Chart *chart);

    void render(const Chart &chart, double seconds);
    void renderFrame(const Chart &chart, double seconds);

    Q_INVOKABLE void renderTime(double seconds);
    Q_INVOKABLE double getNextKeyFrame(double seconds);
    Q_INVOKABLE double getPrevKeyFrame(double seconds);

    const double LOWER_LIMIT_X = -0.5;
    const double UPPER_LIMIT_X = 1.5;
    const double LOWER_LIMIT_Y = -0.5;
    const double UPPER_LIMIT_Y = 1.5;

private:
    LinesModel *judgeLines;
    NotesModel *taps;
    NotesModel *drags;
    NotesModel *flicks;
    HoldsModel *holds;

    Chart *loadedChart;
signals:

};

#endif // FRAMEMANAGER_H
