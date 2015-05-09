#include <QDebug>
#include <QDateTime>

#include "carditemmodel.h"

CardItemModel::CardItemModel(QObject *parent) : QStandardItemModel(parent)
{
    qsrand(QDateTime::currentDateTime().toTime_t());

    QStringList columnLabels;
    columnLabels << "id" << "front" << "back";
    setHorizontalHeaderLabels(columnLabels);
    setColumnCount(columnLabels.size());
}

CardItemModel::~CardItemModel()
{
    qDebug() << "CardItemModel::~CardItemModel";
}

int CardItemModel::id() const
{
    if(rowCount() == 0)
        return -1;
    else
        return item(0,0)->text().toInt();
}

QString CardItemModel::front() const
{
    if(rowCount() == 0)
        return QString();
    else
        return item(0,1)->text();
}

QString CardItemModel::back() const
{
    if(rowCount() == 0)
        return QString();
    else
        return item(0,2)->text();
}


void CardItemModel::shuffle()
{
    for(int i=rowCount(); i>0; i--) {
        int j = qrand() % i;
        for(int c=0;c<columnCount();c++) {
            QStandardItem *iItem = takeItem(i-1,c);
            QStandardItem *jItem = takeItem(j,c);
            setItem(i-1,c,jItem);
            setItem(j,c,iItem);
        }
    }

}

void CardItemModel::removeCard()
{
    if(rowCount() > 0)
        removeRow(0);
}

void CardItemModel::reinsertCard()
{
    if(rowCount() > 0) {
        QList<QStandardItem*> items = takeRow(0);
        insertRow(qMin(7,rowCount()) ,items);
    }
}


