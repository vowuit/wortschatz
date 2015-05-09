#include "qtquick1applicationviewer.h"

#include <QDeclarativeComponent>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>


QtQuick1ApplicationViewer::QtQuick1ApplicationViewer(QWidget *parent)
    : QDeclarativeView(parent)
{
    connect(engine(), SIGNAL(quit()), SLOT(close()));
    setResizeMode(QDeclarativeView::SizeRootObjectToView);
}

QtQuick1ApplicationViewer::~QtQuick1ApplicationViewer()
{
}

QtQuick1ApplicationViewer *QtQuick1ApplicationViewer::create()
{
    return new QtQuick1ApplicationViewer();
}

void QtQuick1ApplicationViewer::setMainQmlFile(const QString &file)
{
    setSource(file);
}

void QtQuick1ApplicationViewer::addImportPath(const QString &path)
{
    engine()->addImportPath(path);
}

void QtQuick1ApplicationViewer::showExpanded()
{
#if defined(Q_WS_SIMULATOR) || defined(Q_OS_QNX)
    showFullScreen();
#else
    show();
#endif
}
