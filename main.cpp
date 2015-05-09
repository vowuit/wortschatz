#include <QApplication>

#ifdef Q_WS_MAEMO_5
#include "maemo5mainwindow.h"
#else
#include "mainwindow.h"
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#ifdef Q_WS_MAEMO_5
    Maemo5MainWindow w;
#else
    MainWindow w;
#endif
    w.show();

    return a.exec();
}
