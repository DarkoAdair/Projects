#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <QString>
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
    QString setSpell1();
    QString setSpell2();

    std::tuple<int, int> start;
    std::tuple<int, int> end;
    std::tuple<int, int> doorCoords;

    QString spellArr1 [4] = {"Vrin", "Gol", "Meind", "Shktov"};
    QString spellArr2 [4] = {"Nim", "Grok", "Val", "Anon"};

public:
    GameMap();
    GameMap(int level);
    std::tuple<int, int> getStart();
    std::tuple<int, int> getEnd();
    int getWhatsAtCoordinate(std::tuple<int,int> coordinates);
    void openDoorWays();
    void killEnemies();

    QString getSpell1();
    QString getSpell2();
    bool guardAsleep();
    std::vector<std::tuple<int, int>> getDoorRange();
    std::vector<std::tuple<int, int>> getEnemyRange();
    std::tuple<int,int> getDoorCoords();
};

#endif // GAMEMAP_H
