
#include <QtWidgets>
#include <QtQuick>
#include <QDebug>

#include "mainwindow.h"
#include "trainer.h"
#include "trainermodel.h"
#include "trainereditor.h"

#include "carditemmodel.h"
#include "wortschatztableview.h"
#include "translationscachedmodel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    trainer(new Trainer(this)),
    activeDockWidget(0)
{

    connect(trainer,SIGNAL(urlChanged()),this,SLOT(updateWindowTitle()));
    connect(trainer,SIGNAL(trainerModelChanged()),this,SLOT(updateEditorViews()));
    connect(trainer,SIGNAL(trainerModelChanged()),this,SLOT(updateTrainerTools()));

    qmlRegisterType<Trainer>("Wortschatz", 1, 0, "Trainer");

    createActions();
    createMenus();
    setDockNestingEnabled(true);
    setUpCentralWidget();
    readSettings();

    connect(qApp,SIGNAL(focusChanged(QWidget*,QWidget*)),this,SLOT(focusChanged(QWidget*,QWidget*)));

}

MainWindow::~MainWindow()
{
}

void MainWindow::openTrainer()
{

    QFileDialog dialog(this,tr("Open File"));
    dialog.setNameFilter(tr("db files (*.db)"));
    dialog.setFileMode(QFileDialog::AnyFile);

    if(!fileName.isEmpty()) {
        QFileInfo fileInfo(fileName);
        dialog.setDirectory(fileInfo.path());
    }

    QStringList fileNames;
    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
        if(fileNames.count() > 0) {
            if(trainer->open(QUrl::fromLocalFile(fileNames.at(0))))
                this->fileName = fileNames.at(0);
        }
    }

}

void MainWindow::closeTrainer()
{
    trainer->close();
    this->fileName = QString();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *ev)
{
    WortschatzTableView *view = qobject_cast<WortschatzTableView *>(obj);
    if(view && ev->type() == QEvent::FocusIn) {
        QDockWidget *dock = qobject_cast<QDockWidget*>(view->parentWidget());
        if(dock != activeDockWidget) {
            if(activeDockWidget != 0)
                activeDockWidget->setWindowTitle(activeDockWidget->windowTitle().replace("*",""));
            dock->setWindowTitle("*" + dock->windowTitle());
            activeDockWidget = dock;

            disconnect(appendRowAct,SIGNAL(triggered()),0,0);
            disconnect(deleteRowAct,SIGNAL(triggered()),0,0);
            disconnect(deleteDataAct,SIGNAL(triggered()),0,0);
            disconnect(submitAllAct,SIGNAL(triggered()),0,0);
            disconnect(revertAllAct,SIGNAL(triggered()),0,0);

            connect(appendRowAct,SIGNAL(triggered()),view,SLOT(appendEntry()));
            connect(deleteRowAct,SIGNAL(triggered()),view,SLOT(deleteRow()));
            connect(deleteDataAct,SIGNAL(triggered()),view,SLOT(deleteData()));
            connect(submitAllAct,SIGNAL(triggered()),view,SLOT(submitAll()));
            connect(revertAllAct,SIGNAL(triggered()),view,SLOT(revertAll()));

        }
    }
    return QMainWindow::eventFilter(obj,ev);
}

void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open File"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(openTrainer()));

    closeAct = new QAction(tr("Close"), this);
    closeAct->setShortcuts(QKeySequence::Close);
    closeAct->setStatusTip(tr("Close File"));
    connect(closeAct, SIGNAL(triggered()), this, SLOT(closeTrainer()));

    quitAct = new QAction(tr("&Quit"), this);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Quit the application"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    shuffleAct = new QAction(tr("&Shuffle"), this);
    shuffleAct->setStatusTip(tr("Shuffle card items"));

    reinitCardsAct = new QAction(tr("&Reinit"), this);
    reinitCardsAct->setStatusTip(tr("Reinit card items"));

    appendRowAct = new QAction(tr("&Insert"), this);
    appendRowAct->setShortcut(Qt::Key_Insert);
    appendRowAct->setStatusTip(tr("Insert new row"));

    deleteRowAct = new QAction(tr("Delete &Row"), this);
    deleteRowAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Delete));
    deleteRowAct->setStatusTip(tr("Delete row"));

    deleteDataAct = new QAction(tr("Delete &Data"), this);
    deleteDataAct->setShortcut(QKeySequence::Delete);
    deleteDataAct->setStatusTip(tr("Delete data"));

    submitAllAct = new QAction(tr("&SubmitAll"), this);
    submitAllAct->setShortcut(QKeySequence::Save);
    submitAllAct->setStatusTip(tr("Submit all changes"));

    revertAllAct = new QAction(tr("Re&vertAll"), this);
    revertAllAct->setShortcut(Qt::Key_Escape);
    revertAllAct->setStatusTip(tr("Revert all changes"));

}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(closeAct);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAct);

    trainerToolsMenu = menuBar()->addMenu(tr("&TrainerTools"));
    trainerToolsMenu->addAction(shuffleAct);
    trainerToolsMenu->addAction(reinitCardsAct);

    editorToolsMenu = menuBar()->addMenu(tr("&EditorTools"));
    editorToolsMenu->addAction(appendRowAct);
    editorToolsMenu->addAction(deleteRowAct);
    editorToolsMenu->addAction(deleteDataAct);
    editorToolsMenu->addAction(submitAllAct);
    editorToolsMenu->addAction(revertAllAct);
}

void MainWindow::setUpCentralWidget()
{
    QQuickView *view = new QQuickView();
    view->setResizeMode(QQuickView::SizeRootObjectToView);

    view->engine()->rootContext()->setContextProperty("trainer",trainer);

    QWidget *container = QWidget::createWindowContainer(view, this);
    container->setFocusPolicy(Qt::TabFocus);
    view->setSource(QUrl("qrc:/main.qml"));

    setCentralWidget(container);
}


void MainWindow::readSettings()
{
    QSettings settings("VoWuIT", "Wortschatz");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());

    if(settings.contains("fileName")) {
        QFile file(settings.value("fileName").toString());
        if(file.exists()) {
            if(trainer->open(QUrl::fromLocalFile(file.fileName())))
                this->fileName = file.fileName();
        } else {
            settings.remove("fileName");
        }
    }
}

void MainWindow::writeSettings()
{
    QSettings settings("VoWuIT", "Wortschatz");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());

    QFile file(fileName);
    if(file.exists())
        settings.setValue("fileName",fileName);
    else
        settings.remove("fileName");

}

QDockWidget* MainWindow::createDockWidget(const QString &objectName)
{
    QDockWidget *dock;

    dock = new QDockWidget(objectName, this);
    dock->setObjectName(objectName);

    if(!restoreDockWidget(dock))
        addDockWidget(Qt::RightDockWidgetArea,dock,Qt::Horizontal);

    return dock;
}


void MainWindow::updateWindowTitle()
{
    if(trainer->url().isEmpty())
        setWindowTitle(qAppName());
    else
        setWindowTitle(trainer->url().toString());
}

void MainWindow::updateEditorViews()
{

    TrainerEditor *editor = qobject_cast<TrainerEditor*>(trainer->trainerModel());
    if(editor) {
        activeDockWidget = 0;
        disconnect(appendRowAct,SIGNAL(triggered()),0,0);
        disconnect(deleteRowAct,SIGNAL(triggered()),0,0);
        disconnect(deleteDataAct,SIGNAL(triggered()),0,0);
        disconnect(submitAllAct,SIGNAL(triggered()),0,0);
        disconnect(revertAllAct,SIGNAL(triggered()),0,0);
        foreach(QAbstractItemModel* model, editor->models()) {

            QDockWidget *dock = findChild<QDockWidget *>(model->objectName());
            if(dock == 0) dock = createDockWidget(model->objectName());

            WortschatzTableView *view = qobject_cast<WortschatzTableView *>(dock->widget());
            if(view == 0) {
                view = new WortschatzTableView(this);
                dock->setWidget(view);
            }

            view->setModel(model);
            view->show();

            TranslationsCachedModel *cachedModel = qobject_cast<TranslationsCachedModel*>(model);
            if(cachedModel) {
                view->installEventFilter(this);
            }
        }
    }

}

void MainWindow::updateTrainerTools()
{
}

void MainWindow::focusChanged(QWidget *, QWidget *)
{

}

/*
void MainWindow::handleHaveFocus()
{
    WortschatzTableView *view = qobject_cast<WortschatzTableView*>(sender());
    QDockWidget *dock = qobject_cast<QDockWidget*>(view->parent());

    dock->setWindowTitle("*" + view->model()->objectName());
    disconnect(appendRowAct,SIGNAL(triggered()));

    //connect(appendRowAct,SIGNAL(triggered()),view,SLOT(appendEntry()));
    //connect(submitAllAct,SIGNAL(triggered()),view->model(),SLOT(submitAll()));
}

void MainWindow::handleLostFocus()
{
    WortschatzTableView *view = qobject_cast<WortschatzTableView*>(sender());
    QDockWidget *dock = qobject_cast<QDockWidget*>(view->parent());

    dock->setWindowTitle(view->model()->objectName());
    //disconnect(appendRowAct,SIGNAL(triggered()),view,SLOT(appendEntry()));

    //disconnect(submitAllAct,SIGNAL(triggered()),view->model(),SLOT(submitAll()));
}
*/

