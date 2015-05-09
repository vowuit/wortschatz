#include "translationssqlmodel.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlResult>

TranslationsSqlModel::TranslationsSqlModel(QObject *parent, const QSqlDatabase &db, const QString &tableName)
    : QSqlQueryModel(parent)
{
    m_db = db;
    m_tableName = tableName;
    setObjectName(tableName);
    refresh();

}

TranslationsSqlModel::~TranslationsSqlModel()
{

}

Qt::ItemFlags TranslationsSqlModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QSqlQueryModel::flags(index);
    if (index.column() == 1 || index.column() == 2)
        flags |= Qt::ItemIsEditable;
    return flags;
}

bool TranslationsSqlModel::setData(const QModelIndex &index, const QVariant &value, int /* role */)
{
    if (index.column() < 1 || index.column() > 2)
        return false;

    QModelIndex primaryKeyIndex = QSqlQueryModel::index(index.row(), 0);
    int id = data(primaryKeyIndex).toInt();

    bool ok = false;
    if(index.column() == 1) {
        ok = updateFromText(id,value.toString());
    } else if(index.column() == 2) {
        ok = updateToText(id,value.toString());
    }
    refresh();
    return ok;
}

bool TranslationsSqlModel::updateFromText(int id, const QString &text)
{
    QSqlQuery query(m_db);
    query.prepare("update texts set text = ? "
                  "where texts.text_id = (select from_text_id from translations where translations.trans_id = ?)");
    query.addBindValue(text);
    query.addBindValue(id);
    return query.exec();
}

bool TranslationsSqlModel::updateToText(int id, const QString &text)
{
    QSqlQuery query(m_db);
    query.prepare("update texts set text = ? "
                  "where texts.text_id = (select to_text_id from translations where translations.trans_id = ?)");
    query.addBindValue(text);
    query.addBindValue(id);
    return query.exec();
}

void TranslationsSqlModel::refresh()
{
    setQuery("SELECT trans_id, from_text, to_text FROM " + m_tableName, m_db);
}
