#include "GameWithMap.h"
#include <iostream>
#include <random>

// 預設建構子：預設建立一個 5x5 地圖
Map::Map() : Map(5, 5) {}

Map::Map(int width, int height) : width(width), height(height), playerX(0), playerY(0) {
    grid.resize(height, std::vector<char>(width, '.'));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, width * height - 1);
    for (int i = 0; i < 3 && i < width * height - 1; i++) {
        int pos = dist(gen);
        int x = pos % width;
        int y = pos / width;
        if (x != playerX || y != playerY) grid[y][x] = 'X';
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

void Map::setPlayerPosition(int x, int y, Player& player) {
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
                player.takeDamage(10);
            } else {
                std::cout << "你找到回血點，回復 20 HP！" << std::endl;
                player.heal(20);
            }
        }
        grid[playerY][playerX] = 'P';
    } else {
        std::cout << "無法移動到該位置！" << std::endl;
    }
}

int Map::getPlayerX() const { return playerX; }
int Map::getPlayerY() const { return playerY; }

GameWithMap::GameWithMap(Player& player) : player(player) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> size_dist(5, 10);
    int size = size_dist(gen);
    map = Map(size, size);  // 現在 map 有預設建構子，可以這樣寫
}

void GameWithMap::start() {
    std::string command;
    while (true) {
        map.display();
        std::cout << "輸入指令 (w - 上, s - 下, a - 左, d - 右, 後接步數, quit - 退出): ";
        std::cin >> command;
        if (command == "quit") {
            break;
        } else if (command.size() >= 2 && (command[0] == 'w' || command[0] == 's' || command[0] == 'a' || command[0] == 'd')) {
            char direction = command[0];
            int steps;
            try {
                steps = std::stoi(command.substr(1));
            } catch (...) {
                std::cout << "無效指令！" << std::endl;
                continue;
            }
            for (int i = 0; i < steps; ++i) {
                int newX = map.getPlayerX();
                int newY = map.getPlayerY();
                if (direction == 'w') newY--;
                else if (direction == 's') newY++;
                else if (direction == 'a') newX--;
                else if (direction == 'd') newX++;
                map.setPlayerPosition(newX, newY, player);
            }
        } else {
            std::cout << "無效指令！" << std::endl;
        }
    }
}
