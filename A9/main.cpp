#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameManager gameEngine;
    MainWindow w(nullptr, &gameEngine);
    w.show();
    return a.exec();
}
