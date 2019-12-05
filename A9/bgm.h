#ifndef BGM_H
#define BGM_H

#include <QMediaPlayer>
#include <QMediaPlaylist>


class Bgm
{
public:
    Bgm();

private:
    QMediaPlaylist *playlist;
    QMediaPlayer *music;

    void musicMute();
    void musicOn();

};

#endif // BGM_H
