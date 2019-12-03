#include "window.h"
#include "ui_window.h"

#include <QApplication>

window::window(GameManager *gameManager) :
    QMainWindow(nullptr),
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
