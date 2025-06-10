#ifndef GAME_WITH_MAP_H
#define GAME_WITH_MAP_H

#include <vector>
#include <string>
#include "Character.h"

class Map {
public:
    enum StoneType { EVOLUTION, REPAIR, DESTINY, HEALING, ATTACK };

    Map();
    Map(int width, int height);
    void display() const;
    bool isWalkable(int x, int y) const;
    void setPlayerPosition(int x, int y, Player& player, int& stonesCollected, bool& stageComplete, StoneType& collectedStone);
    int getPlayerX() const;
    int getPlayerY() const;
    void askQuestion(Player& player);

private:
    int width;
    int height;
    std::vector<std::vector<char>> grid;
    int playerX;
    int playerY;
};

class GameWithMap {
public:
    GameWithMap(Player& player);
    void start(bool fromBattle = false);
    bool hasCollectedTwoStones() const;
    int getTotalStonesCollected() const;

private:
    Map map;
    Player& player;
    int stonesCollected;
    int totalStonesCollected;
};

#endif