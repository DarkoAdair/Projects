#include "gamemap.h"

#define OBJECT_AVAILABLEPATH 0
#define OBJECT_WALL 1
#define OBJECT_SPIKES 2
#define OBJECT_LAVA 3
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
 *  3 is lava
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
   //TODO set up coordinates of path,walls, spikes, enemies, etc.
   // change level picture


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

   // mapCoordinates[0][0] = OBJECT_KEY;
   // mapCoordinates[0][7] = OBJECT_DOORWAYTOOPEN;

    mapCoordinates[0][0] = OBJECT_WEAPON;
    mapCoordinates[0][7] = OBJECT_ENEMY;
}

void GameMap::LoadLevelTwo()
{
    start =  std::make_tuple(2, 0);
    end =  std::make_tuple(9, 9);
    //fill with available space
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

std::tuple<int, int> GameMap::getStart()
{
    return start;
}

std::tuple<int, int> GameMap::getEnd()
{
    return end;
}

// used to see if player walked over spikes, hit enemy, hit walls, etc.
// returns an int which is a representation of whats at a map coordinate.
int GameMap::getWhatsAtCoordinate(std::tuple<int,int> coordinates)
{
    return mapCoordinates[std::get<0>(coordinates)][std::get<1>(coordinates)];

}
