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
    QString setSpell(int phase);
    QString bookSpell1, bookSpell2;
    QString correctSpell1, correctSpell2;

    std::tuple<int, int> start;
    std::tuple<int, int> end;
    std::tuple<int, int> doorCoords;
    std::tuple<int, int> keyCoords;
    std::tuple<int, int> enemyCoords;

    QString spellArr1 [4] = {"Vrin", "Gol", "Meind", "Shktov"};
    QString spellArr2 [4] = {"Nim", "Grok", "Val", "Anon"};
    int spellcastPhase, bookReadPhase;

    int generateRandomNumber(int low, int high);

public:
    GameMap();
    GameMap(int level);
    std::tuple<int, int> getStart();
    std::tuple<int, int> getEnd();
    int getWhatsAtCoordinate(std::tuple<int,int> coordinates);
    void openDoorWays();
    void killEnemies();
    std::tuple<int,int> getDoorCoords();
    std::tuple<int,int> getKeyCoords();
    std::tuple<int,int> getEnemyCoords();

    void incrementSpellcastPhase();
    void incrementBookReadPhase();
    int getBookReadPhase();
    int getSpellcastPhase();
    QString getBookSpell(int phase);
    QString getCorrectSpell(int phase);

    bool guardAsleep();
    std::vector<std::tuple<int, int>> getDoorRange();
    std::vector<std::tuple<int, int>> getEnemyRange();
};

#endif // GAMEMAP_H
