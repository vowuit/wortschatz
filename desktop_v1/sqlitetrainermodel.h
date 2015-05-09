#ifndef SQLITETRAINERMODEL_H
#define SQLITETRAINERMODEL_H

#include <QObject>
#include <QSqlDatabase>

#include "trainermodel.h"
#include "trainereditor.h"

class QSqlQueryModel;
class CardItemModel;

class SqliteTrainerModel : public TrainerModel, public TrainerEditor
{
    Q_OBJECT
    Q_INTERFACES(TrainerEditor)

public:
    explicit SqliteTrainerModel(QObject *parent = 0, QString fileName = QString());
    virtual ~SqliteTrainerModel();

signals:
    void tableCardsUpdated();

    // TrainerModel interface
public:
    QString front() const;
    QString back() const;

public slots:
    bool answer(Trainer::Answer answer);

    // TrainerEditor interface
public:
    QList<QAbstractItemModel *> models() const;

private:
    void initModels();
    void initCardItemModel();

    QSqlDatabase m_db;

    QList<QSqlQueryModel *> m_tableList;
    QList<QSqlQueryModel *> m_viewList;

    QList<QSqlQueryModel *> m_models;

    CardItemModel *m_cardItemModel;
};

#endif // SQLITETRAINERMODEL_H
