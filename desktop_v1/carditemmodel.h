#ifndef CARDITEMMODEL_H
#define CARDITEMMODEL_H

#include <QStandardItemModel>

class CardItemModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit CardItemModel(QObject *parent = 0);
    ~CardItemModel();

    int     id() const;
    QString front() const;
    QString back() const;

signals:

public slots:
    void shuffle();
    void removeCard();
    void reinsertCard();
};

#endif // CARDITEMMODEL_H
