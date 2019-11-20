#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "gamemap.h"
#include "player.h"
#include <QObject>

class GameManager : public QObject
{
    Q_OBJECT

    Player player;
    GameMap level;

public:
    GameManager();

    bool isLevelCompleted();

    Q_INVOKABLE void moveUp(int spaces = 0);
    Q_INVOKABLE void moveDown(int spaces = 0);
    Q_INVOKABLE void moveLeft(int spaces = 0);
    Q_INVOKABLE void moveRight(int spaces = 0);

    //std::tuple<int, int, bool>

public slots:

signals:
    void movePlayer(int x, int y, bool gameOver);
};

#endif // GAMEMANAGER_H
