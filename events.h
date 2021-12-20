#ifndef EVENTS_H
#define EVENTS_H

#include <QJsonObject>

class SpeedEvent {
public:
    double startTime;
    double endTime;
    double floor;
    double value;

    SpeedEvent(double startTime, double endTime, double floor, double value);

    static SpeedEvent fromJsonObject(const QJsonObject &data);
};

class NormalEvent {
public:
    double startTime;
    double endTime;
    double start;
    double end;
    double start2;
    double end2;

    NormalEvent(double startTime, double endTime, double start, double end, double start2, double end2);

    static NormalEvent fromJsonObject(const QJsonObject &data);
    static NormalEvent fromJsonObjectV1(const QJsonObject &data);
};

#endif // EVENTS_H
