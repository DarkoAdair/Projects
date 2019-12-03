#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include "mainwindow.h"
#include "gamemanager.h"
namespace Ui {
class window;
}

class window : public QMainWindow
{
    Q_OBJECT

public:
    window(GameManager* gameManager);
    ~window();

private slots:
    void on_pushButton_clicked();

private:
    Ui::window *ui;
    GameManager *gameManager;
};

#endif // WINDOW_H
