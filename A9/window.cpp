#include "window.h"
#include "ui_window.h"

#include <QApplication>
#include <QMovie>

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
