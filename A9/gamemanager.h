#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "gamemap.h"
#include "player.h"

class gameManager
{
public:
    gameManager();

    bool isLevelCompleted();



private:
    Player user;
    GameMap level;
};

#endif // GAMEMANAGER_H
