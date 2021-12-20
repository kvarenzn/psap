#include "judgeline.h"

#include <QJsonArray>
#include <QtMath>

JudgeLine::JudgeLine()
{

}

JudgeLine JudgeLine::fromJsonObject(const QJsonObject &data) {
    JudgeLine result;

    auto notesAboveObjects = data["notesAbove"].toArray();
    auto notesBelowObjects = data["notesBelow"].toArray();
    result.bpm = data["bpm"].toDouble();
    auto speedEventObjects = data["speedEvents"].toArray();
    auto disappearEventObjects = data["judgeLineDisappearEvents"].toArray();
    auto moveEventObjects = data["judgeLineMoveEvents"].toArray();
    auto rotateEventObjects = data["judgeLineRotateEvents"].toArray();

    for (const auto &item : qAsConst(notesAboveObjects)) {
        result.notesAbove.append(Note::fromJsonObject(item.toObject()));
    }

    for (const auto &item : qAsConst(notesBelowObjects)) {
        result.notesBelow.append(Note::fromJsonObject(item.toObject()));
    }

    for (const auto &item : qAsConst(speedEventObjects)) {
        result.speedEvents.append(SpeedEvent::fromJsonObject(item.toObject()));
    }

    for (const auto &item : qAsConst(disappearEventObjects)) {
        result.disappearEvents.append(NormalEvent::fromJsonObject(item.toObject()));
    }

    for (const auto &item : qAsConst(moveEventObjects)) {
        result.moveEvents.append(NormalEvent::fromJsonObject(item.toObject()));
    }

    for (const auto &item : qAsConst(rotateEventObjects)) {
        result.rotateEvents.append(NormalEvent::fromJsonObject(item.toObject()));
    }

    return result;
}

JudgeLine JudgeLine::fromJsonObjectV1(const QJsonObject &data) {
    JudgeLine result;

    auto speedEventObjects = data["speedEvents"].toArray();
    result.bpm = data["bpm"].toDouble();
    double currentFloor = 0.0;

    for (const auto &item : qAsConst(speedEventObjects)) {
        const auto &event = item.toObject();

        double startTime = event["startTime"].toDouble();
        double endTime = event["endTime"].toDouble();
        double value = event["value"].toDouble();

        double floorPosition;
        if (event.contains("floorPosition")) {
            floorPosition = event["floorPosition"].toDouble();
        } else {
            floorPosition = currentFloor;
            currentFloor += 1.875 * (endTime - startTime) * value / result.bpm;
        }

        result.speedEvents.append(SpeedEvent(startTime, endTime, floorPosition, value));
    }

    auto notesAboveObjects = data["notesAbove"].toArray();
    auto notesBelowObjects = data["notesBelow"].toArray();
    auto disappearEventObjects = data["judgeLineDisappearEvents"].toArray();
    auto moveEventObjects = data["judgeLineMoveEvents"].toArray();
    auto rotateEventObjects = data["judgeLineRotateEvents"].toArray();

    for (const auto &item : qAsConst(notesAboveObjects)) {
        result.notesAbove.append(Note::fromJsonObject(item.toObject()));
    }

    for (const auto &item : qAsConst(notesBelowObjects)) {
        result.notesBelow.append(Note::fromJsonObject(item.toObject()));
    }

    for (const auto &item : qAsConst(disappearEventObjects)) {
        result.disappearEvents.append(NormalEvent::fromJsonObject(item.toObject()));
    }

    for (const auto &item : qAsConst(moveEventObjects)) {
        result.moveEvents.append(NormalEvent::fromJsonObject(item.toObject()));
    }

    for (const auto &item : qAsConst(rotateEventObjects)) {
        result.rotateEvents.append(NormalEvent::fromJsonObject(item.toObject()));
    }

    return result;
}

double JudgeLine::secondsToTime(double seconds) const {
    return seconds * bpm / 1.875;
}

double JudgeLine::timeToSeconds(double time) const {
    return time * 1.875 / bpm;
}

double JudgeLine::floor(double time) const {
    for (const auto &e : qAsConst(speedEvents)) {
        if (e.startTime <= time && time <= e.endTime) {
            return timeToSeconds((time - e.startTime) * e.value) + e.floor;
        }
    }
    throw "floor position not found";
}

double JudgeLine::opacity(double time) const {
    for (const auto &e: qAsConst(disappearEvents)) {
        if (e.startTime <= time && time <= e.endTime) {
            return e.start + (e.end - e.start) * (time - e.startTime) / (e.endTime - e.startTime);
        }
    }
    return 1.0;
}

QPair<double, double> JudgeLine::position(double time) const {
    for (const auto &e: qAsConst(moveEvents)) {
        if (e.startTime <= time && time <= e.endTime) {
            double rate = (time - e.startTime) / (e.endTime - e.startTime);
            return {e.start + (e.end - e.start) * rate,
                        1.0 - (e.start2 + (e.end2 - e.start2) * rate)};
        }
    }
    return {0.5, 0.5};
}

double JudgeLine::angle(double time) const {
    for (const auto &e: qAsConst(rotateEvents)) {
        if (e.startTime <= time && time <= e.endTime) {
            return - (e.start + (e.end - e.start) * (time - e.startTime) / (e.endTime - e.startTime));
        }
    }
    return 0.0;
}

QPair<double, double> JudgeLine::getNotePosition(const Note &note, double time) const {
    double offsetX = note.x / 10;
    auto pos = position(time);
    double a = - angle(time) * M_PI / 180.0;
    return {pos.first + offsetX * qCos(a) * 9 / 16, pos.second + offsetX * qSin(a)};
}

QPair<double, double> JudgeLine::getNotePosition(const Note &note) const {
    return getNotePosition(note, note.timestamp);
}

void JudgeLine::markDualNotes() {
    QHash<double, QList<Note *>> map;
    for (auto &note: notesAbove) {
        auto time = timeToSeconds(note.timestamp);
        if (!map.contains(time)) {
            map.insert(time, QList<Note *>());
        }
        map[time].append(&note);
    }
    for (auto &note: notesBelow) {
        auto time = timeToSeconds(note.timestamp);
        if (!map.contains(time)) {
            map.insert(time, QList<Note *>());
        }
        map[time].append(&note);
    }
    for (auto &list: map) {
        if (list.size() <= 1) {
            continue;
        }
        for (auto &note: list) {
            note->dual = true;
        }
    }
}
