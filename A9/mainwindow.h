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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void movePlayerUp(int increment);

    void movePlayerDown(int increment);

    void movePlayerLeft(int increment);

    void movePlayerRight(int increment);

    void on_goButton_clicked();

    void on_debugButton_clicked();

    void on_debugLeftButton_clicked();

    void on_debugRightButton_clicked();

    void on_debugStopButton_clicked();

private:
    Ui::MainWindow *ui;

    CodeEditor *codeEditor;
    CodeManager *codeManager;

};
#endif // MAINWINDOW_H
