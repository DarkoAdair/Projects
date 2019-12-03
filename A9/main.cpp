#include "mainwindow.h"
#include "window.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    GameManager gameEngine;
    //MainWindow *main(nullptr, &gameEngine);
    window w(nullptr);
    w.show();
    return a.exec();
}
