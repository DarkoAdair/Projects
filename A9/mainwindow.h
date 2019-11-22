#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "codeeditor.h"
#include "codemanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, GameManager *gameEngine = nullptr);
    ~MainWindow();

private slots:
    void on_goButton_clicked();

    void on_debugButton_clicked();

    void on_debugLeftButton_clicked();

    void on_debugRightButton_clicked();

    void on_debugStopButton_clicked();

    //GameManager
    void movePlayer(int x, int y, bool gameOver);

    //CodeManager
    void onDebugLineChanged(int currentLine);
    void onDebugException(const QString errorMessage);
    void onRunningFinsih();

private:
    Ui::MainWindow *ui;

    CodeEditor *codeEditor;
    CodeManager *codeManager;
    GameManager *gameEngine;
    QCompleter *completer;

    QAbstractItemModel *modelFromFile(const QString& fileName);
};
#endif // MAINWINDOW_H
