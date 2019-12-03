#include "window.h"
#include "ui_window.h"

#include <QApplication>

window::window(GameManager *gameManager) :
    QMainWindow(nullptr),
    ui(new Ui::window)
{
    ui->setupUi(this);
    this->gameManager = gameManager;
}

window::~window()
{
    delete ui;
    gameManager = nullptr;
}

void window::on_pushButton_clicked()
{
    close();
    MainWindow *w = new MainWindow(nullptr, gameManager);
    w->show();
}
