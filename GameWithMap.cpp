#include "GameWithMap.h"
#include <iostream>
#include <random>

Map::Map(int width, int height) : width(width), height(height), playerX(0), playerY(0) {
    grid.resize(height, std::vector<char>(width, '.'));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, width * height - 1);
    for (int i = 0; i < 3; i++) {
        int pos = dist(gen);
        int x = pos % width;
        int y = pos / width;
        if (x != playerX || y != playerY) grid[y][x] = 'X'; // 陷阱和血包都用 'X'
    }
    grid[playerY][playerX] = 'P';
}

void Map::display() const {
    for (const auto& row : grid) {
        for (char cell : row) {
            std::cout << cell << ' ';
        }
        std::cout << std::endl;
    }
}

bool Map::isWalkable(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

void Map::setPlayerPosition(int x, int y) {
    if (isWalkable(x, y)) {
        grid[playerY][playerX] = '.';
        playerX = x;
        playerY = y;
        if (grid[playerY][playerX] == 'X') {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> event_dist(0, 1);
            if (event_dist(gen) == 0) {
                std::cout << "你踩中陷阱，損失 10 HP！" << std::endl;
            } else {
                std::cout << "你找到回血點，回復 20 HP！" << std::endl;
            }
        }
        grid[playerY][playerX] = 'P';
    } else {
        std::cout << "無法移動到該位置！" << std::endl;
    }
}

int Map::getPlayerX() const { return playerX; }
int Map::getPlayerY() const { return playerY; }

GameWithMap::GameWithMap() : map(10, 10) {}

void GameWithMap::start() {
    std::string command;
    while (true) {
        map.display();
        std::cout << "輸入指令 (w - 上, s - 下, a - 左, d - 右, quit - 退出): ";
        std::cin >> command;
        if (command == "quit") {
            break;
        } else if (command == "w" || command == "s" || command == "a" || command == "d") {
            int newX = map.getPlayerX();
            int newY = map.getPlayerY();
            if (command == "w") newY--;
            else if (command == "s") newY++;
            else if (command == "a") newX--;
            else if (command == "d") newX++;
            map.setPlayerPosition(newX, newY);
        } else {
            std::cout << "無效指令！" << std::endl;
        }
    }
}