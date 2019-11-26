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
    int levelCount;

public:
    GameManager();

    void loadLevel(int levelNum);
    //void checkLevelCompletionReset();

    Q_INVOKABLE void moveUp(int spaces = 0);
    Q_INVOKABLE void moveDown(int spaces = 0);
    Q_INVOKABLE void moveLeft(int spaces = 0);
    Q_INVOKABLE void moveRight(int spaces = 0);

    // takes in a list of coordinates that a move would propose, sets players coordinates to where the player should be
    // at, like if they run into a wall for example. Returns a bool saying if move killed player or not
    bool checkPathSetActualSpot(std::vector<std::tuple<int, int>> tryingPath);

    // returns an int representing whats the moved-to spot after a player hits go
    int getWhatsAtMove(int x, int y);

    int getLevelCount();


public slots:
    void checkLevelCompletionReset();


signals:
    void movePlayer(int x, int y, bool gameOver);
    void updateLevelAndMap(int levelNum);

};

#endif // GAMEMANAGER_H
