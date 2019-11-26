#include "gamemanager.h"
#include <QDebug>
#include <vector>
#include <tuple>

GameManager::GameManager()  {
    loadLevel(1);
}

bool GameManager::isLevelCompleted()
{
    return (player.getX() == std::get<0>(level.getEnd()) &&
            player.getY() == std::get<1>(level.getEnd()));
}

void GameManager::moveUp(int spaces)
{
    qDebug() << "MOVEUP : " << spaces;

    std::vector<std::tuple<int, int>> traversed = player.moveUp(spaces);
    checkPathSetActualSpot(traversed);

    emit movePlayer(player.getX(),player.getY(),false);
}

void GameManager::moveDown(int spaces)
{
    qDebug() << "MOVEDOWN : " << spaces;

    std::vector<std::tuple<int, int>> traversed = player.moveDown(spaces);
    checkPathSetActualSpot(traversed);

    emit movePlayer(player.getX(),player.getY(),false);
}

void GameManager::moveLeft(int spaces)
{
    qDebug() << "MOVELEFT : " << spaces;

    std::vector<std::tuple<int, int>> traversed = player.moveLeft(spaces);
    checkPathSetActualSpot(traversed);

    emit movePlayer(player.getX(),player.getY(),false);
}

void GameManager::moveRight(int spaces)
{
    qDebug() << "MOVERIGHT : " << spaces;
    
    std::vector<std::tuple<int, int>> traversed = player.moveRight(spaces);
    checkPathSetActualSpot(traversed);

    emit movePlayer(player.getX(),player.getY(),false);
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

void GameManager::checkPathSetActualSpot(std::vector<std::tuple<int, int>> tryingPath)
{
    std::tuple<int, int> actualSpot;

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
                case 2: actualSpot = level.getStart();
                        break;
                // lava
                case 3: actualSpot = level.getStart();
                        break;
                // enemy
                case 4: actualSpot = level.getStart();
                        break;
                // key
                case 5: actualSpot = mapBlock; // stop player at space
                        player.setKey(true);
                        break;
                // weapon
                case 6: actualSpot = mapBlock;
                        player.setWeapon(true);
                        break;
                // doorway
                case 7: // let rtrn stay as last available spot
                        break;
                // spellBook stand
                case 8: // let rtrn stay as last available spot
                        break;
                // endPoint
                case 9: actualSpot = mapBlock;
                        break;
                default: //TODO
                        break;
            }
            break;
        }
    }
    player.setX(std::get<0>(actualSpot));
    player.setY(std::get<1>(actualSpot));
}
