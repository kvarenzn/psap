#include "framemanager.h"

#include <QtMath>

FrameManager::FrameManager(QObject *parent) : QObject(parent)
{

}

void FrameManager::bindModels(LinesModel *judgeLines, NotesModel *taps, NotesModel *drags, NotesModel *flicks, HoldsModel *holds) {
    this->judgeLines = judgeLines;
    this->taps = taps;
    this->drags = drags;
    this->flicks = flicks;
    this->holds = holds;
}

void FrameManager::clear() {
    if (!this->judgeLines)
        return;
    this->judgeLines->clearAll();
    if (!this->taps)
        return;
    this->taps->clearAll();
    if (!this->drags)
        return;
    this->drags->clearAll();
    if (!this->flicks)
        return;
    this->flicks->clearAll();
    if (!this->holds)
        return;
    this->holds->clearAll();
}

void FrameManager::loadChart(Chart *chart) {
    loadedChart = chart;
}

void FrameManager::render(const Chart &chart, double seconds) {
    for (const auto &line: chart.judgeLines) {
        auto timestamp = line.secondsToTime(seconds);
        auto pos = line.position(timestamp);
        auto angle = line.angle(timestamp);
        auto opacity = line.opacity(timestamp);
        this->judgeLines->addJudgeLine(pos.first, pos.second, angle, opacity);

        auto floor = line.floor(timestamp);
        auto a = angle * M_PI / 180;
        auto sa = qSin(a);
        auto ca = qCos(a);

        for (const auto &note: line.notesAbove) {
            if (note.timestamp + note.hold < timestamp) {
                continue;
            }
            auto offsetX = note.x / 10;
            auto offsetY = (floor - note.floor) / 2;

            auto noteX = pos.first + (offsetX * ca - offsetY * sa) * 9 / 16;
            auto noteY = pos.second + offsetX * sa + offsetY * ca;

            if ((note.type != NoteType::HOLD ||
                 (note.type == NoteType::HOLD && timestamp < note.timestamp))
                    &&
                    (noteX < LOWER_LIMIT_X || noteX > UPPER_LIMIT_X || noteY < LOWER_LIMIT_Y || noteY > UPPER_LIMIT_Y)) {
                continue;
            }

            switch (note.type) {
            case NoteType::TAP:
                this->taps->addNote(noteX, noteY, angle, note.dual);
                break;
            case NoteType::DRAG:
                this->drags->addNote(noteX, noteY, angle, note.dual);
                break;
            case NoteType::FLICK:
                this->flicks->addNote(noteX, noteY, angle, note.dual);
                break;
            case NoteType::HOLD: {
                auto height = line.timeToSeconds(note.hold * note.speed) / 2;
                auto passed = 0.0;
                if (timestamp > note.timestamp) {
                    noteX = pos.first + offsetX * ca * 9 / 16;
                    noteY = pos.second + offsetX * sa;
                    passed = (seconds - line.timeToSeconds(note.timestamp)) * note.speed / 2;
                }
                this->holds->addHold(noteX, noteY, angle, height, passed, note.dual);
                break;
            }
            }
        }

        for (const auto &note: line.notesBelow) {
            if (note.timestamp + note.hold < timestamp) {
                continue;
            }
            double offsetX = note.x / 10;
            double offsetY = (note.floor - floor) / 2;

            double noteX = pos.first + (offsetX * ca - offsetY * sa) * 9 / 16;
            double noteY = pos.second + offsetX * sa + offsetY * ca;

            if ((note.type != NoteType::HOLD ||
                 (note.type == NoteType::HOLD && timestamp < note.timestamp))
                    &&
                    (noteX < LOWER_LIMIT_X || noteX > UPPER_LIMIT_X || noteY < LOWER_LIMIT_Y || noteY > UPPER_LIMIT_Y)) {
                continue;
            }

            switch (note.type) {
            case NoteType::TAP:
                this->taps->addNote(noteX, noteY, angle, note.dual);
                break;
            case NoteType::DRAG:
                this->drags->addNote(noteX, noteY, angle, note.dual);
                break;
            case NoteType::FLICK:
                this->flicks->addNote(noteX, noteY, angle, note.dual);
                break;
            case NoteType::HOLD: {
                auto height = line.timeToSeconds(note.hold * note.speed) / 2;
                auto passed = (seconds - line.timeToSeconds(note.timestamp)) / 2;
                if (timestamp > note.timestamp) {
                    noteX = pos.first + offsetX * ca * 9 / 16;
                    noteY = pos.second + offsetX * sa;
                }
                this->holds->addHold(noteX, noteY, - angle, height, passed, note.dual);
                break;
            }
            }
        }
    }
}

void FrameManager::renderFrame(const Chart &chart, double seconds) {
    clear();
    render(chart, seconds);
}

void FrameManager::renderTime(double seconds) {
    clear();
    render(*loadedChart, seconds);
}

double FrameManager::getNextKeyFrame(double seconds) {
    return loadedChart->getNextKeyFrame(seconds) / 1000.0;
}

double FrameManager::getPrevKeyFrame(double seconds) {
    return loadedChart->getPrevKeyFrame(seconds) / 1000.0;
}
