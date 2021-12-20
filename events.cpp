#include "events.h"

SpeedEvent::SpeedEvent(double startTime, double endTime, double floor, double value) {
    this->startTime = startTime;
    this->endTime = endTime;
    this->floor = floor;
    this->value = value;
}

SpeedEvent SpeedEvent::fromJsonObject(const QJsonObject &data) {
    return SpeedEvent(
                data["startTime"].toDouble(),
                data["endTime"].toDouble(),
                data["floorPosition"].toDouble(),
                data["value"].toDouble()
            );
}

NormalEvent::NormalEvent(double startTime, double endTime, double start, double end, double start2, double end2) {
    this->startTime = startTime;
    this->endTime = endTime;
    this->start = start;
    this->end = end;
    this->start2 = start2;
    this->end2 = end2;
}

NormalEvent NormalEvent::fromJsonObject(const QJsonObject &data) {
    return NormalEvent(
                data["startTime"].toDouble(),
            data["endTime"].toDouble(),
            data["start"].toDouble(),
            data["end"].toDouble(),
            data.contains("start2") ? data["start2"].toDouble() : 0.,
            data.contains("end2") ? data["end2"].toDouble() : 0.);
}

NormalEvent NormalEvent::fromJsonObjectV1(const QJsonObject &data) {
    auto start = data["start"].toInt();
    auto end = data["end"].toInt();
    return NormalEvent(
                data["startTime"].toDouble(),
            data["endTime"].toDouble(),
            (start / 1000) / 880.0,
            (end / 1000) / 880.0,
            (start % 1000) / 520.0,
            (end % 1000) / 520.0);
}
