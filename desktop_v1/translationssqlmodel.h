#ifndef TRANSLATIONSSQLMODEL_H
#define TRANSLATIONSSQLMODEL_H

#include <QSqlQueryModel>
#include <QSqlDatabase>

class TranslationsSqlModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit TranslationsSqlModel(QObject *parent=0, const QSqlDatabase &db = QSqlDatabase(), const QString &tableName = QString());
    ~TranslationsSqlModel();

    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int);

private:
    bool updateFromText(int id, const QString& text);
    bool updateToText(int id, const QString& text);
    void refresh();

    QSqlDatabase m_db;
    QString m_tableName;
};

#endif // TRANSLATIONSSQLMODEL_H
