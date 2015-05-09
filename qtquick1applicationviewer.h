#ifndef QTQUICK1APPLICATIONVIEWER_H
#define QTQUICK1APPLICATIONVIEWER_H

#include <QDeclarativeView>

class QtQuick1ApplicationViewer : public QDeclarativeView
{
    Q_OBJECT

public:

    explicit QtQuick1ApplicationViewer(QWidget *parent = 0);
    virtual ~QtQuick1ApplicationViewer();

    static QtQuick1ApplicationViewer *create();

    void setMainQmlFile(const QString &file);
    void addImportPath(const QString &path);

    void showExpanded();
};

#endif // QTQUICK1APPLICATIONVIEWER_H
