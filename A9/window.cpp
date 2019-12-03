#include "window.h"
#include "ui_window.h"

#include <QApplication>

window::window(QWidget *widget) :
    QMainWindow(widget),
    ui(new Ui::window)
{
    ui->setupUi(this);
}

window::~window()
{
    delete ui;
}

void window::on_pushButton_clicked()
{
    close();
    GameManager gameEngine;
    MainWindow *w = new MainWindow(this, &gameEngine);
    w->show();

}
