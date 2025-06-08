<<<<<<< HEAD
#ifndef GAME_WITH_MAP_H
#define GAME_WITH_MAP_H

#include <vector>
#include <string>

class Map {
public:
    Map(int width, int height);
    void display() const;
    bool isWalkable(int x, int y) const;
    void setPlayerPosition(int x, int y);
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
    GameWithMap();
    void start();

private:
    Map map;
};

=======
#ifndef GAME_WITH_MAP_H
#define GAME_WITH_MAP_H

#include <vector>
#include <string>

class Map {
public:
    Map(int width, int height);
    void display() const;
    bool isWalkable(int x, int y) const;
    void setPlayerPosition(int x, int y);
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
    GameWithMap();
    void start();

private:
    Map map;
};

>>>>>>> 1375e4f2c47132a367752508f7f518dbf4a6ab4b
#endif