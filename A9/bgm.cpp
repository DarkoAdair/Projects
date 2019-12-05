#include "bgm.h"

Bgm::Bgm()
{
    playlist = new QMediaPlaylist();
    playlist->addMedia(QUrl("qrc:/ChipTune3.1.mp3"));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    music = new QMediaPlayer();
    music->setPlaylist(playlist);
    music->setVolume(25);
    music->play();
}

void Bgm::musicMute() {
    music->setMuted(true);
}

void Bgm::musicOn() {
    music->setMuted(false);
}

