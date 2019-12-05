#include "window.h"
#include "ui_window.h"
#include "bgm.h"

#include <QApplication>
#include <QMovie>


window::window(GameManager *gameManager) :
    QMainWindow(nullptr),
    ui(new Ui::window)
{
    ui->setupUi(this);

    Bgm();

    this->gameManager = gameManager;

    // gif
    QMovie *dance = new QMovie(":/pixel.gif");

    dance->start();
    ui->gif->setAttribute(Qt::WA_NoSystemBackground);
    ui->gif->setMovie(dance);
    ui->gif->setScaledContents(true);


}


window::~window()
{
    delete ui;
    gameManager = nullptr;
}


void window::on_startButton_clicked()
{
    close();
    MainWindow *w = new MainWindow(nullptr, gameManager);
    w->show();
}

void window::musicMute() {
    qDebug() << "recieved musicMute signal";
    music->setMuted(true);
}

void window::musicOn() {
    music->setMuted(false);
}
