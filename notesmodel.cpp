#include "notesmodel.h"

NotesModel::NotesModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

QHash<int, QByteArray> NotesModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[PositionX] = "posX";
    roles[PositionY] = "posY";
    roles[RotateAngle] = "angle";
    roles[HoldLength] = "holdLength";
    roles[Dual] = "dual";
    return roles;
}

int NotesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return notes.size();
}

QVariant NotesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    auto note = notes[index.row()];

    switch (role) {
    case PositionX:
        return note.positionX;
    case PositionY:
        return note.positionY;
    case RotateAngle:
        return note.rotateAngle;
    case HoldLength:
        return note.holdLength;
    case Dual:
        return note.dual;
    }

    return QVariant();
}

void NotesModel::addNote(double positionX, double positionY, double rotateAngle, bool dual, double holdLength) {
    beginInsertRows(QModelIndex(), notes.size(), notes.size());
    notes.append(NoteItem {
                     positionX,
                     positionY,
                     rotateAngle,
                     holdLength,
                     dual
                 });
    endInsertRows();
}

void NotesModel::clearAll() {
    beginResetModel();
    notes.clear();
    endResetModel();
}
