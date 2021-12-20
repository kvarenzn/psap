#ifndef LINESMODEL_H
#define LINESMODEL_H

#include <QAbstractListModel>

struct JudgeLineItem {
    double positionX;
    double positionY;
    double rotateAngle;
    double opacity;
};

class LinesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum LineRoles {
        PositionX = Qt::UserRole + 1,
        PositionY,
        RotateAngle,
        Opacity
    };

    explicit LinesModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    void addJudgeLine(double positionX, double positionY, double rotateAngle, double opacity = 1.0);
    void clearAll();
private:

    QList<JudgeLineItem> lines;
};

#endif // LINESMODEL_H
