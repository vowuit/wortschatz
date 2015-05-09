#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDriver>
#include <QSqlQueryModel>

#include "sqlitetrainermodel.h"
#include "carditemmodel.h"
#include "translationssqlmodel.h"
#include "translationscachedmodel.h"

#include "sqlite3.h"

static void trace( void* /*arg*/, const char* query )
{
    qDebug() << "SQLite:" << QString::fromUtf8( query );
}

SqliteTrainerModel::SqliteTrainerModel(QObject *parent, QString fileName) :
    TrainerModel(parent)
{

    m_cardItemModel = new CardItemModel(this);
    m_cardItemModel->setObjectName("card_items");

    QSqlError err;
    QSqlDatabase db = QSqlDatabase::database(fileName);
    if(!db.isValid()) {
        db = QSqlDatabase::addDatabase("QSQLITE", fileName);
        db.setDatabaseName(fileName);
    }
    if (!db.open()) {
        err = db.lastError();
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase(fileName);
    }
    if(err.isValid()) {
        qWarning() << err.text();
    } else {
        qDebug() << fileName << "opened";

        m_db = db;

        QVariant v = db.driver()->handle();
        if (v.isValid() && qstrcmp(v.typeName(), "sqlite3*")==0) {
            // v.data() returns a pointer to the handle
            sqlite3 *handle = *static_cast<sqlite3 **>(v.data());
            if (handle != 0) { // check that it is not NULL
                sqlite3_trace(handle,trace,NULL);
            }
        }

        initCardItemModel();

        initModels();

    }

}

SqliteTrainerModel::~SqliteTrainerModel()
{
    qDebug() << "SqliteTrainerModel::~SqliteTrainerModel";

}

QString SqliteTrainerModel::front() const
{
    return m_cardItemModel->front();
}

QString SqliteTrainerModel::back() const
{
    return m_cardItemModel->back();
}

bool SqliteTrainerModel::answer(Trainer::Answer answer)
{

    static const QString update_success =
        "update cards set "
            "box_in_date = datetime('now','localtime'), "
            "box_id = min((select max(boxes.box_id) from boxes), cards.box_id + 1) "
        "where card_id = %1;";

    static const QString update_fail =
        "update cards set "
            "box_in_date = datetime('now','localtime'), "
            "box_id = 0 "
        "where card_id = %1;";


    if(answer == Trainer::SUCCESS) {
        m_db.exec(update_success.arg(m_cardItemModel->id()));
        emit tableCardsUpdated();
        m_cardItemModel->removeCard();
        if(m_cardItemModel->rowCount() == 0) {
            initCardItemModel();
        }
        return true;
    } else if(answer == Trainer::FAIL) {
        m_db.exec(update_fail.arg(m_cardItemModel->id()));
        emit tableCardsUpdated();
        m_cardItemModel->reinsertCard();
        return true;
    } else {
        return false;
    }
}

QList<QAbstractItemModel *> SqliteTrainerModel::models() const
{
    QList<QAbstractItemModel *> _models;

    foreach(QAbstractItemModel *model, m_models)
        _models.append(model);

    foreach(QAbstractItemModel *model, m_tableList)
        _models.append(model);

    foreach(QAbstractItemModel *model, m_viewList)
        _models.append(model);

   _models.append(m_cardItemModel);

    return _models;
}

void SqliteTrainerModel::initModels()
{
    //QSqlQuery tablesAndViews("select name, type from sqlite_master where type in ('table','view') and name not like 'sqlite%'",m_db);

    QStringList tablesAndViews;
    tablesAndViews.append(m_db.tables(QSql::Tables));
    tablesAndViews.append(m_db.tables(QSql::Views));

    foreach(const QString tableName,tablesAndViews) {

        if(tableName.endsWith("_translations")) {
            //TranslationsSqlModel *model = new TranslationsSqlModel(this,m_db,tableName);
            TranslationsCachedModel *model = new TranslationsCachedModel(this,m_db);
            model->setTable(tableName);
            model->setObjectName(tableName);
            model->setEditStrategy(QSqlTableModel::OnManualSubmit);
            model->select();
            m_models.append(model);
        } else {
            QSqlQueryModel *model = new QSqlQueryModel(this);
            model->setQuery("SELECT * FROM " + tableName,m_db);
            model->setObjectName(tableName);
            m_models.append(model);
        }
    }

    /*
    foreach(WortschatzTableModel *tableModel,m_tableList) {
        foreach(WortschatzTableModel *viewModel,m_viewList) {
            viewModel->connect(tableModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(select()));
            viewModel->connect(tableModel,SIGNAL(modelReset()),SLOT(select()));
        }

        if(tableModel->tableName() == "cards") {
            connect(this,SIGNAL(tableCardsUpdated()),tableModel,SLOT(select()));
        }
    }
    */

}

void SqliteTrainerModel::initCardItemModel()
{
    QSqlQuery dueCards("SELECT card_id, front_text, back_text FROM due_text_cards",m_db);
    while(dueCards.next()) {
        QList<QStandardItem *> items;
        items.append(new QStandardItem(dueCards.value("card_id").toString()));
        items.append(new QStandardItem(dueCards.value("front_text").toString()));
        items.append(new QStandardItem(dueCards.value("back_text").toString()));
        m_cardItemModel->appendRow(items);
    }
    m_cardItemModel->shuffle();
}

