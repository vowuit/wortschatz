#ifndef WORTSCHATZDATABASE_H
#define WORTSCHATZDATABASE_H

#include <QSqlDatabase>
#include <QSqlRecord>

class WortschatzDatabase : public QSqlDatabase
{
public:
    explicit WortschatzDatabase();
    explicit WortschatzDatabase(const WortschatzDatabase &other);
    virtual ~WortschatzDatabase();

};

#endif // WORTSCHATZDATABASE_H
