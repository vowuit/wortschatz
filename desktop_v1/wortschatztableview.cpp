#include "wortschatztableview.h"

#include <QDebug>
#include <QKeyEvent>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSet>
#include <QAbstractItemDelegate>

#include "translationscachedmodel.h"

WortschatzTableView::WortschatzTableView(QWidget *parent) :
    QTableView(parent)
{
    installEventFilter(this);
    setEditTriggers(QAbstractItemView::AnyKeyPressed | QAbstractItemView::EditKeyPressed | QAbstractItemView::DoubleClicked);
}

WortschatzTableView::~WortschatzTableView()
{

}

bool WortschatzTableView::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter ) {
            if (selectionModel()->currentIndex().row() == model()->rowCount() - 1) {
                appendEntry();
            }
        }
    }
    // standard event processing
    return QObject::eventFilter(obj, event);
}

void WortschatzTableView::appendEntry()
{
    qDebug() << "WortschatzTableView::appendEntry" << model()->objectName();
    TranslationsCachedModel *_model = qobject_cast<TranslationsCachedModel *>(model());
    if(_model) {
        _model->submit();
        _model->insertRow(model()->rowCount());
        QModelIndex newIndex = _model->index(_model->rowCount()-1,1);
        scrollTo(newIndex);
        selectionModel()->clear(); // otherwise proxy mapping gets screwed for some reason
        selectionModel()->select(newIndex, QItemSelectionModel::ClearAndSelect);
        selectionModel()->setCurrentIndex(newIndex, QItemSelectionModel::ClearAndSelect);
        edit(newIndex);
    }

}

void WortschatzTableView::deleteRow()
{
    QMap<int,int> rowsToBeDeleted;
    foreach(QModelIndex index, selectedIndexes()) {
        rowsToBeDeleted.insert(index.row(),index.row());
    }
    while(rowsToBeDeleted.size()>0) {
        model()->removeRow(rowsToBeDeleted.take(rowsToBeDeleted.lastKey()));
    }
}

void WortschatzTableView::deleteData()
{
    foreach(QModelIndex index, selectedIndexes()) {
        model()->setData(index,QVariant());
    }
}

void WortschatzTableView::submitAll()
{
    QModelIndex index = currentIndex();

    int currentRow = index.row();
    int currentCol = index.column();

    QSqlTableModel *_model = qobject_cast<QSqlTableModel*>(model());
    if(_model) {
        if( state() == EditingState) {
            currentChanged(index,index);
        }
        bool ok = _model->submitAll();

        if(!ok) {
            qWarning() << _model->lastError();
        }

        if(currentRow >= _model->rowCount())
            index = _model->index(_model->rowCount()-1,currentCol);

        scrollTo(index);
        selectionModel()->clear(); // otherwise proxy mapping gets screwed for some reason
        selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
        selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);

    }
}

void WortschatzTableView::revertAll()
{
    QSqlTableModel *_model = qobject_cast<QSqlTableModel*>(model());
    if(_model) {
        _model->revertAll();
    }
}
