#ifndef WORTSCHATZTABLEVIEW_H
#define WORTSCHATZTABLEVIEW_H

#include <QTableView>

class WortschatzTableView : public QTableView
{
    Q_OBJECT

public:
    explicit WortschatzTableView(QWidget *parent = 0);
    virtual ~WortschatzTableView();

    bool eventFilter(QObject *obj, QEvent *event);

public slots:
    void appendEntry();
    void deleteRow();
    void deleteData();
    void submitAll();
    void revertAll();
};

#endif // WORTSCHATZTABLEVIEW_H
