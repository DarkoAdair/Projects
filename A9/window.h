#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include "mainwindow.h"

namespace Ui {
class window;
}

class window : public QMainWindow
{
    Q_OBJECT
    QMediaPlaylist *playlist;
    QMediaPlayer *music;

public:
    explicit window(QWidget *widget);
    ~window();

private slots:
    void on_pushButton_clicked();

private:
    Ui::window *ui;
};

#endif // WINDOW_H
