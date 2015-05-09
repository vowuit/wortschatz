#ifndef WORTSCHATZDRIVERPLUGIN_H
#define WORTSCHATZDRIVERPLUGIN_H

#include <QSqlDriverPlugin>

class WortschatzDriverPlugin : public QSqlDriverPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.wortschatz.Qt.QSqlDriverFactoryInterface" FILE "wortschatz.json")

public:
    WortschatzDriverPlugin();
    ~WortschatzDriverPlugin();

    QSqlDriver *create(const QString &name);
};

#endif // WORTSCHATZDRIVERPLUGIN_H
