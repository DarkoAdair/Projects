#include "gamemap.h"

#include <QDebug>
#include <QString>

#define OBJECT_AVAILABLEPATH 0
#define OBJECT_WALL 1
#define OBJECT_SPIKES 2
#define OBJECT_ENEMYLINEOFSIGHT 3
#define OBJECT_ENEMY 4
#define OBJECT_KEY 5
#define OBJECT_WEAPON 6
#define OBJECT_DOORWAYTOOPEN 7
#define OBJECT_SPELLBOOKSTAND 8
#define OBJECT_ENDPOINT 9


GameMap::GameMap()
{
    //TODO
}

GameMap::GameMap(int level)
{
    switch (level)
    {
        case 1: LoadLevelOne();
                break;
        case 2: LoadLevelTwo();
                break;
        case 3: LoadLevelThree();
                break;
        default: //TODO
                break;
    }
}

/*  Switch Case numbers for objects in gameMap
 *
 *  0 is availablePath
 *  1 is walls
 *  2 is spikes
 *  3 is enemy line of sight
 *  4 is enemies
 *  5 is key
 *  6 is weapons
 *  7 is doorwayToOpen
 *  8 is spellbookStand
 *  9 is endPoint
 *
 */

//  ******** level coordinates set up like [x] [y] with higher x value being further left on a row, and higher y value being lower down cols
// same for tuples

void GameMap::LoadLevelOne()
{
    start =  std::make_tuple(1, 6);
    end =  std::make_tuple(9, 9);
    doorCoords = std::make_tuple(0,7);
    keyCoords = std::make_tuple(0,2);
    enemyCoords = std::make_tuple(-1,-1);

    // TEST
    // fill mapCoordinates with available space
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            mapCoordinates[i][j] = OBJECT_AVAILABLEPATH;
        }
    }

    mapCoordinates[9][9] = OBJECT_ENDPOINT;// set endpoint
    mapCoordinates[0][1] = OBJECT_WALL;
    mapCoordinates[1][1] = OBJECT_WALL;
    mapCoordinates[2][1] = OBJECT_WALL;
    mapCoordinates[3][1] = OBJECT_WALL;
    mapCoordinates[8][2] = OBJECT_WALL;
    mapCoordinates[8][3] = OBJECT_WALL;
    mapCoordinates[3][7] = OBJECT_WALL;
    mapCoordinates[3][8] = OBJECT_WALL;
    mapCoordinates[3][9] = OBJECT_WALL;
    mapCoordinates[4][4] = OBJECT_WALL;
    mapCoordinates[4][5] = OBJECT_WALL;
    mapCoordinates[4][6] = OBJECT_WALL;
    mapCoordinates[4][7] = OBJECT_WALL;
    mapCoordinates[5][7] = OBJECT_WALL;
    mapCoordinates[6][7] = OBJECT_WALL;

    mapCoordinates[0][2] = OBJECT_KEY;
    mapCoordinates[0][7] = OBJECT_DOORWAYTOOPEN;

    //mapCoordinates[0][0] = OBJECT_WEAPON;
    //mapCoordinates[0][7] = OBJECT_ENEMY;
}

void GameMap::LoadLevelTwo()
{
    start =  std::make_tuple(1, 6);
    end =  std::make_tuple(9, 9);
    doorCoords = std::make_tuple(-1,-1);
    keyCoords = std::make_tuple(-1,-1);
    enemyCoords = std::make_tuple(-1,-1);

    //fill with available space
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            mapCoordinates[i][j] = OBJECT_AVAILABLEPATH;
        }
    }
    mapCoordinates[9][9] = OBJECT_ENDPOINT;// set endpoint
    mapCoordinates[0][1] = OBJECT_SPIKES;
    mapCoordinates[1][1] = OBJECT_SPIKES;
    mapCoordinates[2][1] = OBJECT_SPIKES;
    mapCoordinates[3][1] = OBJECT_SPIKES;
    mapCoordinates[8][2] = OBJECT_SPIKES;
    mapCoordinates[8][3] = OBJECT_SPIKES;
    mapCoordinates[3][7] = OBJECT_SPIKES;
    mapCoordinates[3][8] = OBJECT_SPIKES;
    mapCoordinates[3][9] = OBJECT_SPIKES;
    mapCoordinates[4][4] = OBJECT_SPIKES;
    mapCoordinates[4][5] = OBJECT_SPIKES;
    mapCoordinates[4][6] = OBJECT_SPIKES;
    mapCoordinates[4][7] = OBJECT_SPIKES;
    mapCoordinates[5][7] = OBJECT_SPIKES;
    mapCoordinates[6][7] = OBJECT_SPIKES;
}

void GameMap::LoadLevelThree()
{
    start =  std::make_tuple(0, 0);
    end =  std::make_tuple(9, 9);
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            mapCoordinates[i][j] = OBJECT_AVAILABLEPATH;
        }
    }
    mapCoordinates[9][9] = OBJECT_ENDPOINT;// set endpoint

    //TODO set up coordinates of path,walls, spikes, enemies, etc.

    // change level picture
}

void GameMap::LoadLevelFour()
{
    start =  std::make_tuple(5, 9);
    end =  std::make_tuple(5, 4);
    int i = 5;
    for(int j = 5; j < 10; j++)
        {
            mapCoordinates[i][j] = OBJECT_AVAILABLEPATH;
        }
    mapCoordinates[5][4] = OBJECT_ENDPOINT;// set spellbook location

    i = 4;
    for(int j = 0; j < 10; j++)
    {
        mapCoordinates[i][j] = OBJECT_WALL;
        mapCoordinates[i+2][j] = OBJECT_WALL; //only narrow pathway
    }

    QString spell1 = setSpell(1);
    QString spell2 = setSpell(2);

    // change level picture
}

void GameMap::openDoorWays()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (mapCoordinates[i][j] == OBJECT_DOORWAYTOOPEN)
                mapCoordinates[i][j] = OBJECT_AVAILABLEPATH;
        }
    }
}

void GameMap::killEnemies()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (mapCoordinates[i][j] == OBJECT_ENEMY)
                mapCoordinates[i][j] = OBJECT_AVAILABLEPATH;
        }
    }
}

bool GameMap::guardAsleep()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (mapCoordinates[i][j] == OBJECT_ENEMYLINEOFSIGHT)
                return false;
        }
    }
    return true;
}

std::vector<std::tuple<int, int>> GameMap::getDoorRange()
{
    std::vector<std::tuple<int, int>> validSpots;

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (mapCoordinates[i][j] == OBJECT_DOORWAYTOOPEN)
            {
                // get adjacent blocks around doorway that are available
                if(i+1 < 10)
                    if(mapCoordinates[i+1][j] == OBJECT_AVAILABLEPATH)
                        validSpots.push_back(std::tuple<int, int>(i+1, j));

                if(i-1 > 0)
                    if(mapCoordinates[i-1][j] == OBJECT_AVAILABLEPATH)
                        validSpots.push_back(std::tuple<int, int>(i-1, j));

                if(j+1 < 10)
                    if(mapCoordinates[i][j+1] == OBJECT_AVAILABLEPATH)
                        validSpots.push_back(std::tuple<int, int>(i, j+1));

                if(j-1 > 0)
                    if(mapCoordinates[i][j-1] == OBJECT_AVAILABLEPATH)
                        validSpots.push_back(std::tuple<int, int>(i, j-1));
            }
        }
    }
    return validSpots;
}

std::vector<std::tuple<int, int>> GameMap::getEnemyRange()
{
    std::vector<std::tuple<int, int>> validSpots;

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (mapCoordinates[i][j] == OBJECT_ENEMY)
            {
                // get adjacent blocks around doorway that are available
                if(i+1 < 10)
                    if(mapCoordinates[i+1][j] == OBJECT_AVAILABLEPATH)
                        validSpots.push_back(std::tuple<int, int>(i+1, j));

                if(i-1 > 0)
                    if(mapCoordinates[i-1][j] == OBJECT_AVAILABLEPATH)
                        validSpots.push_back(std::tuple<int, int>(i-1, j));

                if(j+1 < 10)
                    if(mapCoordinates[i][j+1] == OBJECT_AVAILABLEPATH)
                        validSpots.push_back(std::tuple<int, int>(i, j+1));

                if(j-1 > 0)
                    if(mapCoordinates[i][j-1] == OBJECT_AVAILABLEPATH)
                        validSpots.push_back(std::tuple<int, int>(i, j-1));
            }
        }
    }


    return validSpots;
}

std::tuple<int, int> GameMap::getStart()
{
    return start;
}

std::tuple<int, int> GameMap::getEnd()
{
    return end;
}

QString GameMap::setSpell(int phase)
{
    int index = generateRandomNumber(0, 3);
    if (phase == 1)
    {
        return spellArr1[index];
    }
    else return spellArr2[index];
}

QString GameMap::getSpell(int phase)
{

}

int GameMap::generateRandomNumber(int low, int high)
{
    return qrand() % ((high + 1) - low) + low;
}



// used to see if player walked over spikes, hit enemy, hit walls, etc.
// returns an int which is a representation of whats at a map coordinate.
int GameMap::getWhatsAtCoordinate(std::tuple<int,int> coordinates)
{
    return mapCoordinates[std::get<0>(coordinates)][std::get<1>(coordinates)];

}

// returns the coordinates of the door in the level if it has one. If no
// door is used, (-1, -1) is returned
std::tuple<int,int> GameMap::getDoorCoords() {
    return doorCoords;
}

// returns the coordinates of the key in the level if it has one. If no
// key is used, (-1, -1) is returned
std::tuple<int,int> GameMap::getKeyCoords() {
    return keyCoords;
}

// returns the coordinates of the enemy in the level if it has one. If no
// enemy is used, (-1, -1) is returned
std::tuple<int,int> GameMap::getEnemyCoords() {
    return enemyCoords;
}
