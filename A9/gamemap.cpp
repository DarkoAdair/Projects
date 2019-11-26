#include "gamemap.h"

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
    start =  std::make_tuple(0, 0);
    end =  std::make_tuple(9, 9);
   //TODO set up coordinates of path,walls, spikes, enemies, etc.
   // change level picture


    // TEST
    // fill mapCoordinates with available space
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            mapCoordinates[i][j] = 0;
        }
    }

    mapCoordinates[9][9] = 9;// set endpoint

     mapCoordinates[4][0] = 3; // set up 4th col on row 0 (top row) as lava

    //mapCoordinates[0][3] = 1; // set up 1st col on row 3 as wall

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
            mapCoordinates[i][j] = 0;
        }
    }
    mapCoordinates[9][9] = 9;// set endpoint



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
            mapCoordinates[i][j] = 0;
        }
    }
    mapCoordinates[9][9] = 9;// set endpoint

    //TODO set up coordinates of path,walls, spikes, enemies, etc.

    // change level picture
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
