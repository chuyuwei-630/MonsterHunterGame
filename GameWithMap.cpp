#include "GameWithMap.h"
#include <iostream>
#include <random>
#include <set>

Map::Map() : Map(10, 10) {} // Default to 10x10 map

Map::Map(int width, int height) : width(width), height(height), playerX(0), playerY(0) {
    grid.resize(height, std::vector<char>(width, '.')); // Initialize empty map
    std::random_device rd;
    std::mt19937 gen(rd());
    std::set<std::pair<int, int>> diamondPositions;

    // Place 25 diamonds randomly, excluding player's starting position (0,0)
    while (diamondPositions.size() < 25) {
        std::uniform_int_distribution<> x_dist(0, width - 1);
        std::uniform_int_distribution<> y_dist(0, height - 1);
        int x = x_dist(gen);
        int y = y_dist(gen);
        if (x != 0 || y != 0) { // Avoid player's starting position
            diamondPositions.insert({x, y});
        }
    }

    // Set diamonds on the grid
    for (const auto& pos : diamondPositions) {
        grid[pos.second][pos.first] = 'D';
    }

    grid[playerY][playerX] = 'P'; // Place player
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

void Map::setPlayerPosition(int x, int y, Player& player, int& diamondsCollected, bool& stageComplete) {
    if (isWalkable(x, y)) {
        grid[playerY][playerX] = '.'; // Clear current position
        playerX = x;
        playerY = y;

        if (grid[playerY][playerX] == 'D') {
            diamondsCollected++;
            grid[playerY][playerX] = 'F'; // Mark collected diamond
            std::cout << "你收集了一顆鑽石！目前收集：" << diamondsCollected << "/2" << std::endl;
            if (diamondsCollected >= 2) {
                stageComplete = true;
                std::cout << "你已收集2顆鑽石，準備返回戰鬥！" << std::endl;
            }
        } else if (grid[playerY][playerX] == 'X') {
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
        grid[playerY][playerX] = 'P'; // Set new player position
    } else {
        std::cout << "無法移動到該位置！" << std::endl;
    }
}

int Map::getPlayerX() const { return playerX; }
int Map::getPlayerY() const { return playerY; }

GameWithMap::GameWithMap(Player& player) : player(player), diamondsCollected(0) {
    map = Map(10, 10); // Always use 10x10 map
}

void GameWithMap::start(bool fromBattle) {
    std::string command;
    bool stageComplete = false;
    diamondsCollected = 0; // Reset for new stage

    while (!stageComplete) {
        map.display();
        std::cout << "輸入指令 (w - 上, s - 下, a - 左, d - 右, 後接步數, quit - 退出): ";
        std::cin >> command;

        if (command == "quit" && !fromBattle) { // Allow quitting only in non-battle mode
            break;
        } else if (command.size() >= 2 && (command[0] == 'w' || command[0] == 's' || command[0] == 'a' || command[0] == 'd')) {
            char direction = command[0];
            int steps;
            try {
                steps = std::stoi(command.substr(1));
                if (steps < 1) {
                    std::cout << "步數必須為正數！" << std::endl;
                    continue;
                }
            } catch (...) {
                std::cout << "無效指令！" << std::endl;
                continue;
            }

            for (int i = 0; i < steps && !stageComplete; ++i) {
                int newX = map.getPlayerX();
                int newY = map.getPlayerY();
                if (direction == 'w') newY--;
                else if (direction == 's') newY++;
                else if (direction == 'a') newX--;
                else if (direction == 'd') newX++;
                map.setPlayerPosition(newX, newY, player, diamondsCollected, stageComplete);
                if (player.getHP() <= 0) {
                    std::cout << "你已死亡，遊戲結束！" << std::endl;
                    stageComplete = true; // Exit map to end game
                }
            }
        } else {
            std::cout << "無效指令！" << std::endl;
        }

        if (stageComplete && fromBattle) {
            break; // Return to battle mode
        }
    }
}

bool GameWithMap::hasCollectedTwoDiamonds() const {
    return diamondsCollected >= 2;
}