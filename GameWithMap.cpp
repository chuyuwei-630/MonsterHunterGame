#include "GameWithMap.h"
#include <iostream>
#include <random>
#include <set>
#include <chrono>
#include <thread>
#include <algorithm>

Map::Map() : Map(10, 10) {}

Map::Map(int width, int height) : width(width), height(height), playerX(0), playerY(0) {
    grid.resize(height, std::vector<char>(width, '.'));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<std::pair<int, int>> positions;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (x != 0 || y != 0) {
                positions.emplace_back(x, y);
            }
        }
    }

    std::shuffle(positions.begin(), positions.end(), gen);

    int posIndex = 0;
    for (int i = 0; i < 25 && posIndex < positions.size(); ++i, ++posIndex) {
        int x = positions[posIndex].first;
        int y = positions[posIndex].second;
        grid[y][x] = 'D';
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

void Map::setPlayerPosition(int x, int y, Player& player, int& stonesCollected, bool& stageComplete, StoneType& collectedStone) {
    if (isWalkable(x, y)) {
        grid[playerY][playerX] = '.';  // 清除當前位置
        playerX = x;
        playerY = y;

        char cell = grid[playerY][playerX];
        if (cell == 'D') {
            askQuestion(player);
            stonesCollected++;
            grid[playerY][playerX] = 'F';  // 收集後改為'F'
            std::cout << "目前收集：" << stonesCollected << "/2" << std::endl;
            if (stonesCollected >= 2) {
                stageComplete = true;
                std::cout << "你已收集2顆鑽石，準備返回戰鬥！" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        } else if (cell == 'F') {
            std::cout << "這個位置的寶石已被收集！" << std::endl;
        }
        grid[playerY][playerX] = 'P';
    } else {
        std::cout << "無法移動到該位置！" << std::endl;
    }
}

void Map::askQuestion(Player& player) {
    std::vector<std::tuple<std::string, std::vector<std::string>, int>> questions = {
        {"現任高雄市市長？", {"韓國瑜", "陳菊", "陳其邁"}, 2},
        {"哪一個縣市不靠海？", {"南投", "台東 ", "台南"}, 0},
        {"美食街有幾間7-11？", {"3", "1", "2"}, 0},
        {"全聯的吉祥物是？", {"奶油獅", "奶龍", "福利熊"}, 2},
        {"LBJ是指甚麼？", {"喇叭間", "劉寶傑", "魯比醬"}, 2},
        {"下列哪一間不是飲料店的品牌？", {"拾九茶屋", "再睡五分鐘", "再給我兩分鐘"}, 2},
        {"哪個不是位在台北市的夜市？", {"竹北夜市", "饒河夜市", "寧夏夜市"}, 0},
        {"下列哪個不會導電？", {"廖老大", "鐵", "塑膠"}, 2},
        {"誰完狼人殺自己當狼人被抓到？", {"邱鋒澤", "陳零九", "黃偉晉"}, 1},
        {"本係沒有哪項運動的系隊？", {"籃球", "彈力球", "羽球"}, 1}
    };
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> q_dist(0, questions.size() - 1);
    int q_idx = q_dist(gen);
    auto [question, options, correct] = questions[q_idx];

    std::cout << "你發現了一顆鑽石！回答以下問題：" << std::endl;
    std::cout << question << std::endl;
    for (size_t i = 0; i < options.size(); ++i) {
        std::cout << i + 1 << ": " << options[i] << std::endl;
    }
    std::cout << "你的答案（1-3）：";
    int answer;
    std::cin >> answer;
    if (answer - 1 == correct) {
        player.heal(15);
        std::cout << "回答正確！回復15 HP！" << std::endl;
    } else {
        player.takeDamage(20);
        std::cout << "回答錯誤，扣除20 HP！" << std::endl;
    }
}

int Map::getPlayerX() const { return playerX; }
int Map::getPlayerY() const { return playerY; }

GameWithMap::GameWithMap(Player& player) : player(player), stonesCollected(0), totalStonesCollected(0) {
    map = Map(10, 10);
}

void GameWithMap::start(bool fromBattle) {
    std::string command;
    bool stageComplete = false;
    stonesCollected = 0;

    while (!stageComplete) {
        map.display();
        std::cout << "輸入指令 (w - 上, s - 下, a - 左, d - 右, 後接步數, quit - 退出): ";
        std::cin >> command;

        if (command == "quit" && !fromBattle) {
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
                Map::StoneType dummyStone;
                map.setPlayerPosition(newX, newY, player, stonesCollected, stageComplete, dummyStone);
                if (stonesCollected > 0) totalStonesCollected++; // 僅在收集寶石時增加總數
                if (player.getHP() <= 0) {
                    std::cout << "你已死亡，遊戲結束！" << std::endl;
                    stageComplete = true;
                }
            }
        } else {
            std::cout << "無效指令！" << std::endl;
        }

        if (stageComplete && fromBattle) {
            break;
        }
    }
}

bool GameWithMap::hasCollectedTwoStones() const {
    return stonesCollected >= 2;
}

int GameWithMap::getTotalStonesCollected() const {
    return totalStonesCollected;
}