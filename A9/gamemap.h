#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <vector>
#include <tuple>


class GameMap
{
private:
    //these will be filled with ints to represent in game things
    int mapCoordinates[10][10];
    void LoadLevelOne();
    void LoadLevelTwo();
    void LoadLevelThree();

    std::tuple<int, int> start;
    std::tuple<int, int> end;
    std::tuple<int, int> doorCoords;



public:
    GameMap();
    GameMap(int level);
    std::tuple<int, int> getStart();
    std::tuple<int, int> getEnd();
    int getWhatsAtCoordinate(std::tuple<int,int> coordinates);
    void openDoorWays();
    void killEnemies();
    bool guardAsleep();
    std::vector<std::tuple<int, int>> getDoorRange();
    std::vector<std::tuple<int, int>> getEnemyRange();
    std::tuple<int,int> getDoorCoords();
};

#endif // GAMEMAP_H
