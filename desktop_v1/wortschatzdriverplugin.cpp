#include "wortschatzdriverplugin.h"

WortschatzDriverPlugin::WortschatzDriverPlugin() : QSqlDriverPlugin()
{

}

WortschatzDriverPlugin::~WortschatzDriverPlugin()
{

}

QSqlDriver *WortschatzDriverPlugin::create(const QString &name)
{
    if (name == QLatin1String("WSQSQLITE")) {
        /*
        QSQLiteDriver* driver = new QSQLiteDriver();
        return driver;
        */
        return 0;
    }
    return 0;
}

