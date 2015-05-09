#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class Trainer;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openTrainer();
    void closeTrainer();
    void updateWindowTitle();
    void updateEditorViews();
    void updateTrainerTools();
    void focusChanged(QWidget*, QWidget*);

protected:
    void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *obj, QEvent *ev);

private:

    void createActions();
    void createMenus();
    void setUpCentralWidget();
    void readSettings();
    void writeSettings();
    QDockWidget *createDockWidget(const QString &objectName);

    QMenu *fileMenu;
    QAction *openAct;
    QAction *closeAct;
    QAction *quitAct;

    QMenu *trainerToolsMenu;
    QAction *shuffleAct;
    QAction *reinitCardsAct;

    QMenu *editorToolsMenu;
    QAction *appendRowAct;
    QAction *deleteRowAct;
    QAction *deleteDataAct;
    QAction *submitAllAct;
    QAction *revertAllAct;

    QString fileName;
    Trainer *trainer;

    QDockWidget *activeDockWidget;
};

#endif // MAINWINDOW_H
