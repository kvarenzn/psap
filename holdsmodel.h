#ifndef HOLDSMODEL_H
#define HOLDSMODEL_H

#include <QAbstractListModel>

struct HoldItem {
    double positionX;
    double positionY;
    double rotateAngle;
    double holdLength;
    double holdPassed;
    bool dual;
};

class HoldsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum HoldRoles {
        PositionX = Qt::UserRole + 1,
        PositionY,
        RotateAngle,
        HoldLength,
        HoldPassed,
        Dual
    };

    explicit HoldsModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    void addHold(double positionX, double positionY, double rotateAngle, double holdLength, double holdPassed = 0.0, bool dual = false);
    void clearAll();

private:

    QList<HoldItem> holds;
};

#endif // HOLDSMODEL_H
