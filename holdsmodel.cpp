#include "holdsmodel.h"

HoldsModel::HoldsModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

QHash<int, QByteArray> HoldsModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[PositionX] = "posX";
    roles[PositionY] = "posY";
    roles[RotateAngle] = "angle";
    roles[HoldLength] = "holdLength";
    roles[HoldPassed] = "holdPassed";
    roles[Dual] = "dual";
    return roles;
}

int HoldsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return holds.size();
}

QVariant HoldsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    auto note = holds[index.row()];

    switch (role) {
    case PositionX:
        return note.positionX;
    case PositionY:
        return note.positionY;
    case RotateAngle:
        return note.rotateAngle;
    case HoldLength:
        return note.holdLength;
    case HoldPassed:
        return note.holdPassed;
    case Dual:
        return note.dual;
    }

    return QVariant();
}

void HoldsModel::addHold(double positionX, double positionY, double rotateAngle, double holdLength, double holdPassed, bool dual) {
    beginInsertRows(QModelIndex(), holds.size(), holds.size());
    holds.append(HoldItem {
                     positionX,
                     positionY,
                     rotateAngle,
                     holdLength,
                     holdPassed,
                     dual
                 });
    endInsertRows();
}

void HoldsModel::clearAll() {
    beginResetModel();
    holds.clear();
    endResetModel();
}
