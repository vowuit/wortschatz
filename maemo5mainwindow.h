#ifndef MAEMO5MAINWINDOW_H
#define MAEMO5MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
QT_END_NAMESPACE

class Maemo5MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit Maemo5MainWindow(QWidget *parent = 0);
    ~Maemo5MainWindow();

private slots:
    void open();
    void showRecent();
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

    QAction *openFileAction;
    QAction *openRecentAction;
    QAction *quitAction;

    QList<QAction*> recentFileActionList;
    const int maxRecentFiles;

    QString currentFilePath;
};

#endif // MAEMO5MAINWINDOW_H
