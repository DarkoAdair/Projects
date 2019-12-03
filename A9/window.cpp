#include "window.h"
#include "ui_window.h"

#include <QApplication>

window::window(QWidget *widget) :
    QMainWindow(widget),
    ui(new Ui::window)
{
    ui->setupUi(this);

    //BGM Player
    playlist = new QMediaPlaylist();
    playlist->addMedia(QUrl("qrc:/ChipTune3.1.mp3"));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    music = new QMediaPlayer();
    music->setPlaylist(playlist);
    music->setVolume(25);
    music->play();

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
