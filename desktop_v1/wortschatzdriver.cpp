#include "wortschatzdriver.h"

#include <QDebug>
#include <QSqlField>

WortschatzDriver::WortschatzDriver(QObject *parent) : QSqlDriver(parent)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
}

WortschatzDriver::~WortschatzDriver()
{
    qDebug() << "WortschatzDriver::~WortschatzDriver";
}

bool WortschatzDriver::hasFeature(QSqlDriver::DriverFeature f) const
{
    return m_db.driver()->hasFeature(f);
}

bool WortschatzDriver::open(const QString &db, const QString &user, const QString &password, const QString &host, int port, const QString &connOpts)
{
    return m_db.driver()->open(db,user,password,host,port,connOpts);
}

void WortschatzDriver::close()
{
    return m_db.driver()->close();
}

QSqlResult *WortschatzDriver::createResult() const
{
    return m_db.driver()->createResult();
}

bool WortschatzDriver::beginTransaction()
{
    return m_db.driver()->beginTransaction();
}

bool WortschatzDriver::commitTransaction()
{
    return m_db.driver()->commitTransaction();
}

bool WortschatzDriver::rollbackTransaction()
{
    return m_db.driver()->rollbackTransaction();
}

QStringList WortschatzDriver::tables(QSql::TableType tableType) const
{
    return m_db.driver()->tables(tableType);
}

QSqlRecord WortschatzDriver::record(const QString &tablename) const
{
    qDebug() << "WortschatzDriver::record" << tablename;
    QSqlRecord rec = m_db.driver()->record(tablename);
    if(tablename.endsWith("_translations")) {
        QSqlField fld = rec.field("trans_id");
        fld.setAutoValue(true);
        rec.replace(rec.indexOf("trans_id"),fld);
    }
    qDebug() << rec;
    for (int c = 0; c < rec.count(); ++c) {
            if (rec.field(c).isAutoValue())
                qDebug() << rec.fieldName(c) << "auto";
    }
    return rec;
}

QSqlIndex WortschatzDriver::primaryIndex(const QString &table) const
{
    qDebug() << "WortschatzDriver::primaryIndex" << table;
    QSqlIndex idx;
    if(table.endsWith("_translations")) {
        QSqlRecord rec = record(table);
        QSqlField fld = rec.field("trans_id");
        idx.append(fld);
    } else {
        idx = m_db.driver()->primaryIndex(table);
    }
    qDebug() << idx;
    return idx;
}

QVariant WortschatzDriver::handle() const
{
    return m_db.driver()->handle();
}

QString WortschatzDriver::escapeIdentifier(const QString &identifier, QSqlDriver::IdentifierType identifierType) const
{
    return m_db.driver()->escapeIdentifier(identifier, identifierType);
}



