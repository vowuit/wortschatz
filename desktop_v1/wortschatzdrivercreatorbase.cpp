#include "wortschatzdrivercreatorbase.h"
#include "wortschatzdriver.h"

#include <QDebug>

WortschatzDriverCreatorBase::WortschatzDriverCreatorBase(QObject *parent) :
    QObject(parent), QSqlDriverCreatorBase()
{

}

WortschatzDriverCreatorBase::~WortschatzDriverCreatorBase()
{
    qDebug() << "WortschatzDriverCreatorBase::~WortschatzDriverCreatorBase()";
}

QSqlDriver *WortschatzDriverCreatorBase::createObject() const
{
    qDebug() << "WortschatzDriverCreatorBase::createObject";
    WortschatzDriver *driver = new WortschatzDriver();
    connect(this,SIGNAL(destroyed()),driver,SLOT(deleteLater()));
    return driver;
}

