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

void GameMap::LoadLevelOne()
{
   //TODO set up coordinates of path,walls, spikes, enemies, etc.

   // change level picture
}

void GameMap::LoadLevelTwo()
{
    //TODO set up coordinates of path,walls, spikes, enemies, etc.

    // change level picture
}

void GameMap::LoadLevelThree()
{
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
