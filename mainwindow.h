#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void open();
    void openRecent();

protected:
    void closeEvent(QCloseEvent *event);

private:
    void createActions();
    void createMenus();
    void setUpCentralWidget();
    void readSettings();
    void writeSettings();

    void loadFile(const QString& filePath);
    void adjustForCurrentFile(const QString& filePath);
    void updateRecentActionList();

    QMenu *fileMenu;
    QAction *openFileAction;
    QAction *quitAction;

    QMenu *recentFilesMenu;
    QList<QAction*> recentFileActionList;
    const int maxRecentFiles;

    QString currentFilePath;

};

#endif // MAINWINDOW_H
