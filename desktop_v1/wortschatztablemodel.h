#ifndef WORTSCHATZTABLEMODEL_H
#define WORTSCHATZTABLEMODEL_H

#include <QObject>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QSet>

class WortschatzTableModel : public QSqlTableModel
{
    Q_OBJECT
    Q_ENUMS(TableType)

public:
    explicit WortschatzTableModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());
    virtual ~WortschatzTableModel();

    enum TableType { UNKNOWN, TABLE, VIEW };
    TableType tableType() const;

    void setTable(const QString &tableName);
    bool select();
    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool insertRow(int row, const QModelIndex &parent = QModelIndex());

    bool insertRowIntoTable(const QSqlRecord &values);
    bool updateRowInTable(int row, const QSqlRecord &values);
    bool deleteRowFromTable(int row);

    bool insertRowIntoTranslations(const QSqlRecord &values);

private:
    TableType m_tableType;
    QSet<int> m_idColumns;
};

#endif // WORTSCHATZTABLEMODEL_H
