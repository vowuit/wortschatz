#include "translationscachedmodel.h"

#include <QDebug>
#include <QSqlQuery>

TranslationsCachedModel::TranslationsCachedModel(QObject *parent, QSqlDatabase db) :
    QSqlTableModel(parent,db)
{

}

TranslationsCachedModel::~TranslationsCachedModel()
{

}

Qt::ItemFlags TranslationsCachedModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags _flags = QSqlTableModel::flags(index);

    if (index.column() == 1 || index.column() == 2)
        return _flags;
    else
        return Qt::NoItemFlags;
}

bool TranslationsCachedModel::updateRowInTable(int row, const QSqlRecord &values)
{
    Q_UNUSED(row);

    database().transaction();

    QSqlQuery updateFromTextQuery(database());
    updateFromTextQuery.prepare("update texts set text = ? "
                  "where texts.text_id = (select from_text_id from translations where translations.trans_id = ?)");
    updateFromTextQuery.addBindValue(values.value("from_text"));
    updateFromTextQuery.addBindValue(values.value("trans_id"));

    if(!updateFromTextQuery.exec()) {
        database().rollback();
        return false;
    }

    QSqlQuery updateToTextQuery(database());
    updateToTextQuery.prepare("update texts set text = ? "
                  "where texts.text_id = (select to_text_id from translations where translations.trans_id = ?)");
    updateToTextQuery.addBindValue(values.value("to_text"));
    updateToTextQuery.addBindValue(values.value("trans_id"));

    if(!updateToTextQuery.exec()) {
        database().rollback();
        return false;
    }

    return database().commit();
}

bool TranslationsCachedModel::insertRowIntoTable(const QSqlRecord &values)
{
    QRegExp rx("([a-z]*)_([a-z]*)_translations");

    if(!rx.exactMatch(tableName()))
        return false;

    QString fromShort = rx.cap(1);
    QString toShort = rx.cap(2);

    database().transaction();

    QSqlQuery insertFromText = QSqlQuery(database());
    insertFromText.prepare("INSERT INTO texts (lang_id, text) "
                           "SELECT lang_id, :from_text FROM languages WHERE short = :from_short");
    insertFromText.bindValue(":from_text",values.value("from_text"));
    insertFromText.bindValue(":from_short",fromShort);

    if(!insertFromText.exec()) {
        database().rollback();
        return false;
    }

    int fromTextId = insertFromText.lastInsertId().toInt();

    QSqlQuery insertToText = QSqlQuery(database());
    insertToText.prepare("INSERT INTO texts (lang_id, text) "
                         "SELECT lang_id, :to_text FROM languages WHERE short = :to_short");
    insertToText.bindValue(":to_text",values.value("to_text"));
    insertToText.bindValue(":to_short",toShort);

    if(!insertToText.exec()) {
        database().rollback();
        return false;
    }

    int toTextId = insertToText.lastInsertId().toInt();

    QSqlQuery insertTranslation = QSqlQuery(database());
    insertTranslation.prepare("INSERT INTO translations (from_text_id, to_text_id) VALUES (:from_text_id, :to_text_id)");
    insertTranslation.bindValue(":from_text_id",fromTextId);
    insertTranslation.bindValue(":to_text_id",toTextId);

    if(!insertTranslation.exec()) {
        database().rollback();
        return false;
    }

    //int transId = insertTranslation.lastInsertId().toInt();

    return database().commit();

}

bool TranslationsCachedModel::deleteRowFromTable(int row)
{
    QSqlRecord values = record(row);

    QSqlQuery deleteTransQuery = QSqlQuery(database());
    deleteTransQuery.prepare("DELETE FROM translations WHERE trans_id = ?");
    deleteTransQuery.addBindValue(values.value("trans_id"));
    return deleteTransQuery.exec();
}

