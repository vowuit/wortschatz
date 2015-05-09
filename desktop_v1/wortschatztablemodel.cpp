#include "wortschatztablemodel.h"

#include <QSqlRecord>
#include <QSqlIndex>
#include <QSqlField>
#include <QSqlError>
#include <QSqlQuery>
#include <QRegExp>
#include <QDebug>

#include "sqlite3.h"

WortschatzTableModel::WortschatzTableModel(QObject *parent, QSqlDatabase db) :
    QSqlTableModel(parent,db),
    m_tableType(UNKNOWN)
{

}

WortschatzTableModel::~WortschatzTableModel()
{
    qDebug() << "WortschatzTableModel::~WortschatzTableModel" << objectName();
}

void WortschatzTableModel::setTable(const QString &tableName)
{
    QSqlTableModel::setTable(tableName);
    setObjectName(tableName);

    QStringList tableNames = database().tables(QSql::Tables);

    if(tableNames.contains(tableName)) {
        m_tableType = TABLE;
    } else {
        QStringList viewNames = database().tables(QSql::Views);
        if(viewNames.contains(tableName)) {
            m_tableType = VIEW;
        } else {
            m_tableType = UNKNOWN;
        }
    }

    m_idColumns.clear();


}

bool WortschatzTableModel::select()
{
    bool ok = QSqlTableModel::select();

    QSqlRecord _record = record();
    if( !_record.isEmpty() ){
        m_idColumns.clear();
        for(int i=0; i<_record.count(); i++) {
            if(_record.fieldName(i).endsWith("_id")) {
                m_idColumns.insert(i);
            }
        }
    }

    if(m_tableType == VIEW) {
        if(tableName().endsWith("_translations")) {
            QSqlIndex ind;
            QSqlField fld("trans_id",QVariant::Int);
            fld.setAutoValue(true);
            fld.setRequired(false);
            ind.append(fld);
            setPrimaryKey(ind);
        }
    }

    return ok;
}

WortschatzTableModel::TableType WortschatzTableModel::tableType() const
{
    return m_tableType;
}

Qt::ItemFlags WortschatzTableModel::flags(const QModelIndex &index) const
{
    if(m_idColumns.contains(index.column()))
        return Qt::NoItemFlags;

    if(tableName().endsWith("_translations"))
        return QSqlTableModel::flags(index);

    if(m_tableType == VIEW)
        return QSqlTableModel::flags(index) ^ Qt::ItemIsEditable;
    else
        return QSqlTableModel::flags(index);
}

bool WortschatzTableModel::insertRow(int row, const QModelIndex &parent)
{
    if(m_tableType == TABLE)
        return QSqlTableModel::insertRow(row, parent);

    if(m_tableType == VIEW && tableName().endsWith("_translations")) {
        bool ok = QSqlTableModel::insertRow(row, parent);
        return ok;
    } else {
        return false;
    }

}

bool WortschatzTableModel::insertRowIntoTable(const QSqlRecord &values)
{
    bool ok = QSqlTableModel::insertRowIntoTable(values);
    qDebug() << "WortschatzTableModel::insertRowIntoTable" << ok << values;
    if(!ok) {
        if(lastError().isValid())
            qDebug() << lastError().text();
    }
    return ok;
    /*
    if(m_tableType == VIEW && tableName().endsWith("_translations"))
        //return insertRowIntoTranslations(values);
        return QSqlTableModel::insertRowIntoTable(values);
    else
        return QSqlTableModel::insertRowIntoTable(values);
   */
}

bool WortschatzTableModel::updateRowInTable(int row, const QSqlRecord &values)
{
    qDebug() << "WortschatzTableModel::updateRowInTable" << row << values;
    return QSqlTableModel::updateRowInTable(row,values);
}

bool WortschatzTableModel::deleteRowFromTable(int row)
{
    qDebug() << "WortschatzTableModel::deleteRowFromTable" << row;
    return QSqlTableModel::deleteRowFromTable(row);
}

bool WortschatzTableModel::insertRowIntoTranslations(const QSqlRecord &values)
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

    database().commit();
    return true;
}





