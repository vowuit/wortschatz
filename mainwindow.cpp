#include "mainwindow.h"

#include <QtGui>
#include <QDebug>
#include <QDeclarativeEngine>

#include "qtquick1applicationviewer.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    maxRecentFiles(5)
{

    createActions();
    createMenus();
    setDockNestingEnabled(true);
    setUpCentralWidget();
    readSettings();

    QTimer::singleShot(5, recentFileActionList.at(0), SLOT(trigger()));

}

MainWindow::~MainWindow()
{
}

void MainWindow::open()
{
    QString dir = QDir::homePath();

    QFileInfo fileInfo(currentFilePath);
    if(fileInfo.exists())
        dir = fileInfo.absolutePath();

    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Wortschatz"), dir, tr("Wortschatz (*.db)"));
    if (!filePath.isEmpty()) loadFile(filePath);
}

void MainWindow::openRecent()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if(action && !action->data().isNull())
        loadFile(action->data().toString());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}

void MainWindow::createActions()
{
    openFileAction = new QAction(tr("&Open"), this);
    openFileAction->setShortcuts(QKeySequence::Open);
    openFileAction->setStatusTip(tr("Open File"));
    connect(openFileAction, SIGNAL(triggered()), SLOT(open()));

    QAction* recentFileAction = 0;
    for(int i = 0; i < maxRecentFiles; i++){
        recentFileAction = new QAction(this);
        recentFileAction->setVisible(false);
        connect(recentFileAction, SIGNAL(triggered()), this, SLOT(openRecent()));
        recentFileActionList.append(recentFileAction);
    }

    quitAction = new QAction(tr("&Quit"), this);
    quitAction->setShortcuts(QKeySequence::Quit);
    quitAction->setStatusTip(tr("Quit the application"));
    connect(quitAction, SIGNAL(triggered()), SLOT(close()));


}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openFileAction);

    recentFilesMenu = fileMenu->addMenu(tr("Open &Recent"));
    for(int i = 0; i < maxRecentFiles; i++)
        recentFilesMenu->addAction(recentFileActionList.at(i));

    fileMenu->addSeparator();
    fileMenu->addAction(quitAction);

    updateRecentActionList();

}

void MainWindow::setUpCentralWidget()
{
    QtQuick1ApplicationViewer *viewer = new QtQuick1ApplicationViewer();
    viewer->addImportPath(QLatin1String("modules"));
    viewer->setMainQmlFile(QLatin1String("qrc:/main.qml"));
    viewer->showFullScreen();

    QObject::connect(viewer->engine(),SIGNAL(quit()), SLOT(close()));

    setCentralWidget(viewer);

}


void MainWindow::readSettings()
{
    QSettings settings("VoWuIT", "Wortschatz");
#ifdef Q_WS_X11
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
#else
    Q_UNUSED(settings)
#endif
}

void MainWindow::writeSettings()
{
    QSettings settings("VoWuIT", "Wortschatz");
#ifdef Q_WS_X11
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
#else
    Q_UNUSED(settings)
#endif
}

void MainWindow::loadFile(const QString &filePath)
{
    QFileInfo fileInfo(filePath);

    if (!fileInfo.exists()) {

        QMessageBox warning(this);
        warning.setWindowTitle(tr("Wortschatz"));
        warning.setText(tr("File does not exist:\n%1.").arg(filePath));
        warning.setIcon(QMessageBox::Warning);
        warning.addButton(QMessageBox::Ok);
        warning.exec();

        return;
    }

    adjustForCurrentFile(filePath);
}

void MainWindow::adjustForCurrentFile(const QString &filePath)
{
    currentFilePath = filePath;
    setWindowFilePath(currentFilePath);

    QSettings settings("VoWuIT", "Wortschatz");
    QStringList recentFilePaths = settings.value("recentFiles").toStringList();
    recentFilePaths.removeAll(filePath);
    recentFilePaths.prepend(filePath);
    while (recentFilePaths.size() > maxRecentFiles)
        recentFilePaths.removeLast();
    settings.setValue("recentFiles", recentFilePaths);

    updateRecentActionList();
}

void MainWindow::updateRecentActionList()
{
    QSettings settings("VoWuIT", "Wortschatz");
    QStringList recentFilePaths =
            settings.value("recentFiles").toStringList();

    int itEnd = 0;
    if(recentFilePaths.size() <= maxRecentFiles)
        itEnd = recentFilePaths.size();
    else
        itEnd = maxRecentFiles;

    for (int i = 0; i < itEnd; i++) {
        QString strippedName = QFileInfo(recentFilePaths.at(i)).fileName();
        recentFileActionList.at(i)->setText(strippedName);
        recentFileActionList.at(i)->setData(recentFilePaths.at(i));
        recentFileActionList.at(i)->setVisible(true);
    }

    for (int i = itEnd; i < maxRecentFiles; i++)
        recentFileActionList.at(i)->setVisible(false);
}
