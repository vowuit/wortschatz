#ifndef WORTSCHATZDRIVER_H
#define WORTSCHATZDRIVER_H

#include <QSqlDriver>
#include <QSqlResult>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlIndex>

class WortschatzDriver : public QSqlDriver
{
public:
    explicit WortschatzDriver(QObject *parent = 0);
    ~WortschatzDriver();

    bool hasFeature(DriverFeature f) const;
    bool open(const QString &db, const QString &user, const QString &password, const QString &host, int port, const QString &connOpts);
    void close();
    QSqlResult *createResult() const;
    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();
    QStringList tables(QSql::TableType tableType) const;

    QSqlRecord record(const QString& tablename) const;
    QSqlIndex primaryIndex(const QString &table) const;
    QVariant handle() const;
    QString escapeIdentifier(const QString &identifier, IdentifierType identifierType) const;

private:
    QSqlDatabase m_db;
};

#endif // WORTSCHATZDRIVER_H
