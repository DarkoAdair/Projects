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
    void LoadLevelFour();
    std::string setSpell1();
    std::string setSpell2();

    std::tuple<int, int> start;
    std::tuple<int, int> end;



public:
    GameMap();
    GameMap(int level);
    std::tuple<int, int> getStart();
    std::tuple<int, int> getEnd();
    int getWhatsAtCoordinate(std::tuple<int,int> coordinates);
    void openDoorWays();
    void killEnemies();

    std::string getSpell1();
    std::string getSpell2();
};

#endif // GAMEMAP_H
