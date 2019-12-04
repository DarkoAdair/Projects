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
    bool isLevelCompleted();
    void resetPlayer();


    Q_INVOKABLE void moveUp(int spaces = 0);
    Q_INVOKABLE void moveDown(int spaces = 0);
    Q_INVOKABLE void moveLeft(int spaces = 0);
    Q_INVOKABLE void moveRight(int spaces = 0);
    Q_INVOKABLE void useKey();
    Q_INVOKABLE void useWeapon();
    Q_INVOKABLE bool checkGuardIsAwake();
    Q_INVOKABLE void wait();

    int getPlayerX();
    int getPlayerY();
    int startX = 1;
    int startY = 6;
    int moveCount = 0;

    //std::tuple<int, int, bool>
    // takes in a list of coordinates that a move would propose, sets players coordinates to where the player should be
    // at, like if they run into a wall for example. Returns a bool saying if move killed player or not
    bool checkPathSetActualSpot(std::vector<std::tuple<int, int>> tryingPath);

    // returns an int representing whats the moved-to spot after a player hits go
    int getWhatsAtMove(int x, int y);

    int getLevelCount();
    bool inRangeOfDoor();
    bool inRangeOfEnemy();
    void triggerGuardSleepState();



    std::tuple<int,int> getDoorCoords();


public slots:
    void checkLevelCompletionReset();
    void emitGameOverSignals();



signals:
    void movePlayer(int x, int y, bool mainComman, bool gameOver);
    void updateLevelCount(int levelNum);
    void resetSignal();
    void useKeySignal();
    void useWeaponSignal();
    void updateInventory(int pickup, bool status);
    void deadSignal(int x, int y);
    void tutorial(int level);
    void toggleEnemyState(int state);



};

#endif // GAMEMANAGER_H
