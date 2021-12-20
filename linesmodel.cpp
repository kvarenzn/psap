#include "linesmodel.h"

LinesModel::LinesModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int LinesModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return lines.size();
}

QHash<int, QByteArray> LinesModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[PositionX] = "posX";
    roles[PositionY] = "posY";
    roles[RotateAngle] = "angle";
    roles[Opacity] = "lineOpacity";
    return roles;
}

QVariant LinesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    auto line = lines[index.row()];
    switch (role) {
    case PositionX:
        return line.positionX;
    case PositionY:
        return line.positionY;
    case RotateAngle:
        return line.rotateAngle;
    case Opacity:
        return line.opacity;
    }

    return QVariant();
}

void LinesModel::addJudgeLine(double positionX, double positionY, double rotateAngle, double opacity) {
    beginInsertRows(QModelIndex(), lines.size(), lines.size());
    lines.append(JudgeLineItem {
                     positionX,
                     positionY,
                     rotateAngle,
                     opacity
                 });
    endInsertRows();
}

void LinesModel::clearAll() {
    beginResetModel();
    lines.clear();
    endResetModel();
}
