#ifndef GAME_WITH_MAP_H
#define GAME_WITH_MAP_H

#include <vector>
#include <string>
#include "Character.h"

class Map {
public:
    Map();  // 預設建構子
    Map(int width, int height);
    void display() const;
    bool isWalkable(int x, int y) const;
    void setPlayerPosition(int x, int y, Player& player, int& diamondsCollected, bool& stageComplete);
    int getPlayerX() const;
    int getPlayerY() const;

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
    void start(bool fromBattle = false); // Modified to track if called from battle
    bool hasCollectedTwoDiamonds() const; // Check if stage is complete

private:
    Map map;
    Player& player;
    int diamondsCollected; // Track diamonds collected in this stage
};

#endif