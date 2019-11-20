#include "gamemanager.h"
#include <QDebug>

GameManager::GameManager()  {

}

bool GameManager::isLevelCompleted()
{
    return (player.getX() == std::get<0>(level.getEnd()) &&
            player.getY() == std::get<1>(level.getEnd()));
}

void GameManager::moveUp(int spaces) {
    qDebug() << "MOVEUP : " << spaces;
    player.setY(player.getY()-spaces);
    emit movePlayer(player.getX(),player.getY(),false);
}

void GameManager::moveDown(int spaces) {
    qDebug() << "MOVEDOWN : " << spaces;
    player.setY(player.getY()+spaces);
    emit movePlayer(player.getX(),player.getY(),false);
}

void GameManager::moveLeft(int spaces) {
    qDebug() << "MOVELEFT : " << spaces;
    player.setX(player.getX()-spaces);
    emit movePlayer(player.getX(),player.getY(),false);
}

void GameManager::moveRight(int spaces) {
    qDebug() << "MOVERIGHT : " << spaces;
    player.setX(player.getX()+spaces);
    emit movePlayer(player.getX(),player.getY(),false);
}
