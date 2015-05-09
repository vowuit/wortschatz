#ifndef WORTSCHATZDRIVERCREATORBASE_H
#define WORTSCHATZDRIVERCREATORBASE_H

#include <QObject>
#include <QSqlDriverCreatorBase>

class WortschatzDriverCreatorBase : public QObject, public QSqlDriverCreatorBase
{
public:
    explicit WortschatzDriverCreatorBase(QObject *parent = 0);
    virtual ~WortschatzDriverCreatorBase();

    QSqlDriver *createObject() const;
};

#endif // WORTSCHATZDRIVERCREATORBASE_H
