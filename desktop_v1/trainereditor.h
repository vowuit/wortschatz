#ifndef TRAINEREDITOR_H
#define TRAINEREDITOR_H

#include <QObject>
#include <QtPlugin>

class QAbstractItemModel;

class TrainerEditor
{
public:
    virtual QList<QAbstractItemModel *> models() const = 0;

};

Q_DECLARE_INTERFACE(TrainerEditor, "TrainerEditor")

#endif // TRAINEREDITOR_H
