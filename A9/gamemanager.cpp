#include "gamemap.h"
#include "player.h"
#include "gamemanager.h"
#include <QDebug>
#include <vector>
#include <tuple>
#include <QtCore>
#include <QString>

GameManager::GameManager()  {

levelCount = 1;
loadLevel(levelCount);
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
    }
    emitGameOverSignals();
}

void GameManager::resetPlayer() {
    std::tuple<int, int> start = level.getStart();
    player.setX(std::get<0>(start));
    player.setY(std::get<1>(start));
}

void GameManager::moveUp(int spaces)
{
    qDebug() << "[GameManager] MOVEUP : " << spaces;

    triggerGuardSleepState();

    std::vector<std::tuple<int, int>> traversed = player.moveUp(spaces);
    bool gameOver = checkPathSetActualSpot(traversed);

    emit movePlayer(player.getX(),player.getY(),true,gameOver);
    emit turnPlayer(1);
}

void GameManager::moveDown(int spaces)
{
    qDebug() << "[GameManager] MOVEDOWN : " << spaces;

    triggerGuardSleepState();

    std::vector<std::tuple<int, int>> traversed = player.moveDown(spaces);
    bool gameOver = checkPathSetActualSpot(traversed);

    emit movePlayer(player.getX(),player.getY(),true,gameOver);
    emit turnPlayer(3);
}

void GameManager::moveLeft(int spaces)
{
    qDebug() << "[GameManager] MOVELEFT : " << spaces;

    triggerGuardSleepState();

    std::vector<std::tuple<int, int>> traversed = player.moveLeft(spaces);
    bool gameOver = checkPathSetActualSpot(traversed);

    emit movePlayer(player.getX(),player.getY(),true,gameOver);
    emit turnPlayer(2);
}

void GameManager::moveRight(int spaces)
{
    qDebug() << "[GameManager] MOVERIGHT : " << spaces;

    triggerGuardSleepState();

    std::vector<std::tuple<int, int>> traversed = player.moveRight(spaces);
    bool gameOver = checkPathSetActualSpot(traversed);

    emit movePlayer(player.getX(),player.getY(),true,gameOver);
    emit turnPlayer(0);
}

void GameManager::wait()
{
    qDebug() << "[GameManager] WAIT ";

    triggerGuardSleepState();
    stayInSpotProceedCode();
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
    moveCount = 0;
}


void GameManager::triggerGuardSleepState()
{
    moveCount++;

    if(moveCount % 4 < 2)
    {
        level.turnGuardAsleep();
        emit toggleEnemyState(0);
    }
    else
    {
        level.turnGuardAwake();
        emit toggleEnemyState(1);
    }
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
                // enemy line of sight
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
    if(inRangeOfDoor())
    {
        if(player.hasAKey())
        {
           qDebug() << "[GameManager] USEKEY : true";
           level.openDoorWays();
           emit useKeySignal();
        }
        else
           qDebug() << "[GameManager] USEKEY : false";
    }
    triggerGuardSleepState();
    stayInSpotProceedCode();
 }

void GameManager::useWeapon()
{
    if(inRangeOfEnemy())
    {
        if(player.canAttack())
        {
           qDebug() << "[GameManager] USEWEAPON : true";
           level.killEnemies();
           emit useWeaponSignal();
        }
        else
           qDebug() << "[GameManager] USEWEAPON : false";
     }
     triggerGuardSleepState();
     stayInSpotProceedCode();
 }


QString GameManager::spellBookRead()
{
    if(!spellBookActive())
    {
        emit deadSignal(player.getX(), player.getY());
        emitGameOverSignals();



        return "";
    }
    else
    {
        if(level.getBookReadPhase() == 3) //tried reading an invalid, extra line
        {
            emit deadSignal(player.getX(), player.getY());
            emitGameOverSignals();
        }
        else
        {
            level.incrementBookReadPhase();
        }
        return (level.getBookSpell(level.getBookReadPhase()-1)); //pre-incrementing
    }
 }

 void GameManager::spellBookCast(QString cast)
 {
     if(!spellBookActive())
     {
         emit deadSignal(player.getX(), player.getY());
         emitGameOverSignals();
     }
     else
     {
         if(level.getSpellcastPhase() == 3) //tried reading an invalid, extra line
         {
             emit deadSignal(player.getX(), player.getY());
             emitGameOverSignals();
         }

         if (cast != (level.getCorrectSpell(level.getSpellcastPhase()))) //not returning, use base index
         {
             emit deadSignal(player.getX(), player.getY());
             emitGameOverSignals();
         }
         else
         {
            emit playerCastSpell(level.getSpellcastPhase());
            level.incrementSpellcastPhase();
          }
     }
 }

 bool GameManager::spellBookActive()
 {
    return player.getX() == 4 && player.getY() == 4 && levelCount == 4; //player should be in center
}


bool GameManager::checkGuardIsAwake()
{
    //moveCount--;
   // triggerGuardSleepState();
    stayInSpotProceedCode();
    return level.guardAwake();
}

bool GameManager::inRangeOfDoor(){
    std::vector<std::tuple<int, int>> validSpots = level.getDoorRange();

    for (std::tuple<int, int> coordinate : validSpots)
    {
        if(player.getX() == std::get<0>(coordinate) && player.getY() == std::get<1>(coordinate))
        return true;
    }
    return false;
}


bool GameManager::inRangeOfEnemy(){
    std::vector<std::tuple<int, int>> validSpots = level.getEnemyRange();


    for (std::tuple<int, int> coordinate : validSpots)
    {
        if(player.getX() == std::get<0>(coordinate) && player.getY() == std::get<1>(coordinate))
            return true;
    }
    return false;
}


std::tuple<int,int> GameManager::getDoorCoords() {
    return level.getDoorCoords();
}

std::tuple<int,int> GameManager::getKeyCoords() {
    return level.getKeyCoords();
}

std::tuple<int,int> GameManager::getEnemyCoords() {
    return level.getEnemyCoords();
}

std::tuple<int,int> GameManager::getSwordCoords() {
    return level.getSwordCoords();
}

std::tuple<int,int> GameManager::getEnd() {
    return level.getEnd();
}

std::tuple<int,int> GameManager::getStart() {
    return level.getStart();
}

 void GameManager::stayInSpotProceedCode()
 {

     std::vector<std::tuple<int, int>> spot;
     spot.push_back(std::tuple<int, int>(player.getX(), player.getY()));
     bool gameOver = checkPathSetActualSpot(spot);

     emit movePlayer(player.getX(),player.getY(),true,gameOver);
 }
