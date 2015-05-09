#ifndef TRANSLATIONSCACHEDMODEL_H
#define TRANSLATIONSCACHEDMODEL_H

#include <QSqlTableModel>
#include <QSqlRecord>

class TranslationsCachedModel : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit TranslationsCachedModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());
    virtual ~TranslationsCachedModel();

    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool updateRowInTable(int row, const QSqlRecord &values);
    bool insertRowIntoTable(const QSqlRecord &values);
    bool deleteRowFromTable(int row);
};

#endif // TRANSLATIONSCACHEDMODEL_H
