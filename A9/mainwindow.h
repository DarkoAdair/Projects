#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "codeeditor.h"

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

private:
    Ui::MainWindow *ui;

    CodeEditor *codeEditor;

};
#endif // MAINWINDOW_H
