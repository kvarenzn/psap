#include "chart.h"

#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <QtMath>

Chart::Chart()
{

}

Chart Chart::fromJsonObject(const QJsonObject &data) {
    Chart result;
    result.version = data["formatVersion"].toInt();
    result.offset = data["offset"].toDouble();
    result.numOfNotes = data["numOfNotes"].toInt();

    auto list = data["judgeLineList"].toArray();
    if (result.version == 1) {
        for (const auto &line: qAsConst(list)) {
            result.judgeLines.append(JudgeLine::fromJsonObjectV1(line.toObject()));
        }
    } else if (result.version == 3) {
        for (const auto &line: qAsConst(list)) {
            result.judgeLines.append(JudgeLine::fromJsonObject(line.toObject()));
        }
    } else {
        throw "Unknown formatVersion number";
    }
    return result;
}

Chart Chart::loadFile(const QString &filepath) {
    QFile jsonFile(filepath);
    jsonFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QString json = jsonFile.readAll();
    jsonFile.close();

    QJsonParseError error;
    auto document = QJsonDocument::fromJson(json.toUtf8(), &error);
    if (error.error != QJsonParseError::NoError) {
        qDebug() << "???" << error.errorString();
        return Chart();
    }

    return fromJsonObject(document.object());
}

void Chart::markDualNotes() {
    QHash<double, QList<Note *>> map;
    for (auto &line: judgeLines) {
        for (auto &note: line.notesAbove) {
            auto time = line.timeToSeconds(note.timestamp);
            if (!map.contains(time)) {
                map.insert(time, QList<Note *>());
            }
            map[time].append(&note);
        }

        for (auto &note: line.notesBelow) {
            auto time = line.timeToSeconds(note.timestamp);
            if (!map.contains(time)) {
                map.insert(time, QList<Note *>());
            }
            map[time].append(&note);
        }
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

void Chart::preprocess() {
    singleNotesByCs.clear();
    holdsByCs.clear();
    for (auto &line: judgeLines) {
        for (auto &note: line.notesAbove) {
            auto seconds = line.timeToSeconds(note.timestamp);
            int css = qFloor(seconds * 1000);

            auto pos = line.getNotePosition(note);
            auto alpha = line.angle(note.timestamp);

            int holdTimeCs = qCeil(line.timeToSeconds(note.hold) * 1000);

            SingleNoteInfo info {
                pos.first,
                pos.second,
                alpha,
                holdTimeCs,
                &note
            };

            if (singleNotesByCs.contains(css)) {
                auto &list = singleNotesByCs[css];
                list.append(info);
            } else {
                singleNotesByCs.insert(css, QList<SingleNoteInfo>() << info);
            }

            if (note.type == NoteType::HOLD) {
                for (int i = 1; i < holdTimeCs; i ++) {
                    auto newCss = css + holdTimeCs;
                    auto newTimestamp = line.secondsToTime(newCss / 1000.0);

                    auto pos = line.getNotePosition(note, newTimestamp);
                    auto alpha = line.angle(newTimestamp);

                    HoldNoteInfo info {
                        css,
                        pos.first,
                        pos.second,
                        alpha,
                        &note
                    };

                    if (holdsByCs.contains(newCss)) {
                        auto &list = holdsByCs[newCss];
                        list.append(info);
                    } else {
                        holdsByCs.insert(newCss, QList<HoldNoteInfo>() << info);
                    }
                }
            }
        }

        for (auto &note: line.notesBelow) {
            auto seconds = line.timeToSeconds(note.timestamp);
            int css = qFloor(seconds * 1000);

            auto pos = line.getNotePosition(note);
            auto alpha = line.angle(note.timestamp);

            int holdTimeCs = qCeil(line.timeToSeconds(note.hold) * 1000);

            SingleNoteInfo info {
                pos.first,
                pos.second,
                alpha,
                holdTimeCs,
                &note
            };

            if (singleNotesByCs.contains(css)) {
                auto &list = singleNotesByCs[css];
                list.append(info);
            } else {
                singleNotesByCs.insert(css, QList<SingleNoteInfo>() << info);
            }

            if (note.type == NoteType::HOLD) {
                for (int i = 1; i < holdTimeCs; i ++) {
                    auto newCss = css + holdTimeCs;
                    auto newTimestamp = line.secondsToTime(newCss / 1000.0);

                    auto pos = line.getNotePosition(note, newTimestamp);
                    auto alpha = line.angle(newTimestamp);

                    HoldNoteInfo info {
                        css,
                        pos.first,
                        pos.second,
                        alpha,
                        &note
                    };

                    if (holdsByCs.contains(newCss)) {
                        auto &list = holdsByCs[newCss];
                        list.append(info);
                    } else {
                        holdsByCs.insert(newCss, QList<HoldNoteInfo>() << info);
                    }
                }
            }
        }
    }
}

int Chart::getNextKeyFrame(double seconds) {
    auto css = qFloor(seconds * 1000);
    auto it = singleNotesByCs.lowerBound(css);
    if (it.key() == css) {
        it ++;
    }
    if (it == singleNotesByCs.end()) {
        it --;
    }
    return it.key();
}

int Chart::getPrevKeyFrame(double seconds) {
    auto css = qFloor(seconds * 1000);
    auto it = singleNotesByCs.lowerBound(css);
    if (it == singleNotesByCs.begin()) {
        return 0;
    }
    return (-- it).key();
}
