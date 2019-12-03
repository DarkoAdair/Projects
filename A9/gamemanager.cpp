#include "gamemanager.h"
#include <QDebug>
#include <vector>
#include <tuple>
#include <QtCore>

GameManager::GameManager()  {
    levelCount = 1;
    loadLevel(1);
}

void GameManager::emitGameOverSignals()
{
    emit updateInventory(0, false);
    emit updateInventory(1, false);
    emit updateLevelCount(getLevelCount());

    // set player item values to false
    player.setKey(false);
    player.setWeapon(false);
}

// method to reset player after dead or level completion.
void GameManager::checkLevelCompletionReset()
{
    // if the player completes the level
    if(player.getX() == std::get<0>(level.getEnd()) &&
           player.getY() == std::get<1>(level.getEnd()))
    {
        levelCount++;
        loadLevel(levelCount);

        // move player to start, whether its because of failure or to the start of a new level
        std::tuple<int, int> start = level.getStart();
        player.setX(std::get<0>(start));
        player.setY(std::get<1>(start));

        QTimer::singleShot(1000, this, SIGNAL(resetSignal()));
        QTimer::singleShot(1000, this, SLOT(emitGameOverSignals()));

    }
    else // player died
    {
        emit deadSignal(player.getX(), player.getY());
        emitGameOverSignals();
    }
}

void GameManager::resetPlayer() {
    std::tuple<int, int> start = level.getStart();
    player.setX(std::get<0>(start));
    player.setY(std::get<1>(start));
}

void GameManager::moveUp(int spaces)
{
    qDebug() << "MOVEUP : " << spaces;

    std::vector<std::tuple<int, int>> traversed = player.moveUp(spaces);
    bool gameOver = checkPathSetActualSpot(traversed);

    emit movePlayer(player.getX(),player.getY(),true,gameOver);
}

void GameManager::moveDown(int spaces)
{
    qDebug() << "MOVEDOWN : " << spaces;

    std::vector<std::tuple<int, int>> traversed = player.moveDown(spaces);
    bool gameOver = checkPathSetActualSpot(traversed);

    emit movePlayer(player.getX(),player.getY(),true,gameOver);
}

void GameManager::moveLeft(int spaces)
{
    qDebug() << "MOVELEFT : " << spaces;

    std::vector<std::tuple<int, int>> traversed = player.moveLeft(spaces);
    bool gameOver = checkPathSetActualSpot(traversed);

    emit movePlayer(player.getX(),player.getY(),true,gameOver);
}

void GameManager::moveRight(int spaces)
{
    qDebug() << "MOVERIGHT : " << spaces;

    std::vector<std::tuple<int, int>> traversed = player.moveRight(spaces);
    bool gameOver = checkPathSetActualSpot(traversed);

    emit movePlayer(player.getX(),player.getY(),true,gameOver);
}

int GameManager::getPlayerX() {
    return player.getX();
}

int GameManager::getPlayerY() {
    return player.getY();
}


void GameManager::loadLevel(int levelNum)
{
    level = GameMap(levelNum);
}

bool GameManager::checkPathSetActualSpot(std::vector<std::tuple<int, int>> tryingPath)
{
    std::tuple<int, int> actualSpot;
    bool gameOver = false;

    // check if there were any objects in the way of that coordinate and set player accordingly
    for (std::tuple<int, int> mapBlock: tryingPath)
    {
        // if the checking space is available path
        if(level.getWhatsAtCoordinate(mapBlock) == 0)
            actualSpot = mapBlock;
        else
        {
            switch (level.getWhatsAtCoordinate(mapBlock))
            {
                // player hit a wall
                case 1: // do nothing, let rtrn stay as last available spot
                        break;
                // spikes
                case 2: actualSpot = mapBlock;
                        gameOver = true;
                        break;
                // lava
                case 3: actualSpot = mapBlock;
                        gameOver = true;
                        break;
                // enemy
                case 4: actualSpot = mapBlock;
                        gameOver = true;
                        break;
                // key
                case 5: actualSpot = mapBlock; // stop player at space
                        player.setKey(true);
                        emit updateInventory(0, true);
                        continue;
                // weapon
                case 6: actualSpot = mapBlock;
                        player.setWeapon(true);
                        emit updateInventory(1, true);
                        continue;
                // doorway
                case 7: // let rtrn stay as last available spot
                        break;
                // spellBook stand
                case 8: // let rtrn stay as last available spot
                        break;
                // endPoint
                case 9: actualSpot = mapBlock;
                        gameOver = true;
                        break;
                default: //TODO
                        break;
            }
            break;
        }
    }
    player.setX(std::get<0>(actualSpot));
    player.setY(std::get<1>(actualSpot));
    return  gameOver;
}

 int GameManager::getWhatsAtMove(int x, int y)
 {
     return level.getWhatsAtCoordinate(std::tuple<int, int>(x, y));
 }

 int GameManager::getLevelCount()
 {
     return levelCount;
 }

 void GameManager::useKey()
 {
    if(player.hasAKey())
    {
       qDebug() << "USEKEY : true";
       level.openDoorWays();
       emit useKeySignal();
    }
    else
       qDebug() << "USEKEY : false";
 }

 void GameManager::useWeapon()
 {
    if(player.canAttack())
    {
       qDebug() << "USEWEAPON : true";
       level.killEnemies();
       emit useKeySignal();
    }
    else
       qDebug() << "USEWEAPON : false";
 }
