#include "wortschatzdatabase.h"

#include <QDebug>

WortschatzDatabase::WortschatzDatabase() : QSqlDatabase()
{

}

WortschatzDatabase::WortschatzDatabase(const WortschatzDatabase &other) : QSqlDatabase(other)
{

}

WortschatzDatabase::~WortschatzDatabase()
{

}


