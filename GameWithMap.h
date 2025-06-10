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
    void setPlayerPosition(int x, int y, Player& player);
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
    void start();

private:
    Map map;
    Player& player;
};

#endif

