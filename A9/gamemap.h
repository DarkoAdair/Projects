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
    std::tuple<int, int> doorCoords;



public:
    GameMap();
    GameMap(int level);
    std::tuple<int, int> getStart();
    std::tuple<int, int> getEnd();
    int getWhatsAtCoordinate(std::tuple<int,int> coordinates);
    void openDoorWays();
    void killEnemies();
<<<<<<< HEAD

    std::string getSpell1();
    std::string getSpell2();
=======
    bool guardAsleep();
    std::vector<std::tuple<int, int>> getDoorRange();
    std::vector<std::tuple<int, int>> getEnemyRange();
    std::tuple<int,int> getDoorCoords();
>>>>>>> 4e91c842095a76b92b2937d41d3e86139c50ecd0
};

#endif // GAMEMAP_H
