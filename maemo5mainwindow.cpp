#include "maemo5mainwindow.h"

#include <QtGui>
#include <QDebug>
#include <QDeclarativeEngine>
#include <QMaemo5ListPickSelector>
#include "qtquick1applicationviewer.h"

Maemo5MainWindow::Maemo5MainWindow(QWidget *parent) :
    QMainWindow(parent),
    maxRecentFiles(5)
{
    createActions();
    createMenus();
    setUpCentralWidget();
    readSettings();

    setAttribute(Qt::WA_Maemo5AutoOrientation);

    QTimer::singleShot(5, recentFileActionList.at(0), SLOT(trigger()));
}

Maemo5MainWindow::~Maemo5MainWindow()
{

}

void Maemo5MainWindow::open()
{
    QString dir = QDir::homePath();

    QFileInfo fileInfo(currentFilePath);
    if(fileInfo.exists())
        dir = fileInfo.absolutePath();

    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Wortschatz"), dir, tr("Wortschatz (*.db)"));
    if (!filePath.isEmpty()) loadFile(filePath);
}

void Maemo5MainWindow::showRecent()
{
    QStandardItemModel model(0, 1);
    for(int i = 0; i < maxRecentFiles; i++){
        QAction *action = recentFileActionList.at(i);
        if(action->isVisible()) {
            QStandardItem *item = new QStandardItem(action->text());
            item->setTextAlignment(Qt::AlignCenter);
            item->setEditable(false);
            model.appendRow(item);
        }
    }

    QMaemo5ListPickSelector *selector = new QMaemo5ListPickSelector;
    selector->setModel(&model);

    QDialog *dialog = qobject_cast<QDialog*>(selector->widget(this));
    if(dialog)
        dialog->exec();

    if(selector->currentIndex() > -1) {
        QAction *action = recentFileActionList.at(selector->currentIndex());
        action->trigger();
    }
}

void Maemo5MainWindow::openRecent()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if(action && !action->data().isNull())
        loadFile(action->data().toString());
}

void Maemo5MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}

void Maemo5MainWindow::createActions()
{
    openFileAction = new QAction(tr("&Open"), this);
    openFileAction->setShortcuts(QKeySequence::Open);
    openFileAction->setStatusTip(tr("Open File"));
    connect(openFileAction, SIGNAL(triggered()), SLOT(open()));

    openRecentAction = new QAction(tr("Open Recent"), this);
    openRecentAction->setShortcuts(QKeySequence::Open);
    openRecentAction->setStatusTip(tr("Open Recent File"));
    connect(openRecentAction, SIGNAL(triggered()), SLOT(showRecent()));

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

void Maemo5MainWindow::createMenus()
{
    menuBar()->addAction(openFileAction);
    menuBar()->addAction(openRecentAction);
    menuBar()->addAction(quitAction);

    updateRecentActionList();
}

void Maemo5MainWindow::setUpCentralWidget()
{
    QtQuick1ApplicationViewer *viewer = new QtQuick1ApplicationViewer();
    viewer->addImportPath(QLatin1String("modules"));
    viewer->setMainQmlFile(QLatin1String("qrc:/main.qml"));
    viewer->showFullScreen();

    QObject::connect(viewer->engine(),SIGNAL(quit()), SLOT(close()));

    setCentralWidget(viewer);
}

void Maemo5MainWindow::readSettings()
{
}

void Maemo5MainWindow::writeSettings()
{
}

void Maemo5MainWindow::loadFile(const QString &filePath)
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

void Maemo5MainWindow::adjustForCurrentFile(const QString &filePath)
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

void Maemo5MainWindow::updateRecentActionList()
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

