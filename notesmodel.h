#ifndef NOTESMODEL_H
#define NOTESMODEL_H

#include <QAbstractListModel>

struct NoteItem {
    double positionX;
    double positionY;
    double rotateAngle;
    double holdLength;
    bool dual;
};

class NotesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum NoteRoles {
        PositionX = Qt::UserRole + 1,
        PositionY,
        RotateAngle,
        HoldLength,
        Dual
    };

    explicit NotesModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    void addNote(double positionX, double positionY, double rotateAngle, bool dual = false, double holdLength = 0.0);
    void clearAll();

private:

    QList<NoteItem> notes;
};

#endif // NOTESMODEL_H
