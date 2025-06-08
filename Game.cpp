#include "Game.h"
#include "GameWithMap.h"
#include <iostream>
#include <random>
#include <thread>
#include <chrono>
#include <vector>

class Monster : public Character {
public:
    Monster(const std::string& name, int hp, int attack, int scoreValue)
        : Character(name, hp, attack), scoreValue(scoreValue) {}
    int getScoreValue() const { return scoreValue; }
private:
    int scoreValue;
};

class Player : public Character {
public:
    Player(const std::string& name, int hp, int attack)
        : Character(name, hp, attack) {}
    void heal(int amount) {
        hp += amount;
        if (hp > 100) hp = 100;
    }
};

class Treasure {
public:
    Treasure() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> type_dist(1, 3);
        type = type_dist(gen);
        if (type == 1) value = 20; // 回血
        else if (type == 2) value = 5; // 攻擊力
        else value = 10; // 積分
    }
    void apply(Player* player, int& score) {
        if (type == 1) {
            player->heal(value);
            slowPrint("你獲得了回血寶箱，回復 " + std::to_string(value) + " HP！");
        } else if (type == 2) {
            player->increaseAttack(value);
            slowPrint("你獲得了攻擊力寶箱，攻擊力增加 " + std::to_string(value) + "！");
        } else {
            score += value;
            slowPrint("你獲得了積分寶箱，增加 " + std::to_string(value) + " 積分！");
        }
    }
private:
    int type;
    int value;
};

Game::Game() {
    player = std::make_unique<Player>("杭特", 100, 25);
    score = 0;
    generateMonster();
}

Game::~Game() {}

void Game::generateMonster() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> hp_dist(50, 100);
    std::uniform_int_distribution<> attack_dist(10, 25);
    std::uniform_int_distribution<> monster_dist(1, 4);

    int hp = hp_dist(gen);
    int atk = attack_dist(gen);
    int monsterType = monster_dist(gen);

    if (monsterType == 1)
        monster = std::make_unique<Monster>("哥布林", hp, atk, 10);
    else if (monsterType == 2)
        monster = std::make_unique<Monster>("雷射", hp, atk + 5, 15);
    else if (monsterType == 3)
        monster = std::make_unique<Monster>("地精", hp, atk, 12);
    else
        monster = std::make_unique<Monster>("巨魔", hp + 20, atk + 10, 20);
    
}


void slowPrint(const std::string& text, int delay) {
    for (char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
    std::cout << std::endl;
}

void Game::start() {
    slowPrint("== 遊戲開始 ==");
    bool gameRunning = true;
    while (gameRunning && player->isAlive()) {
        if (score >= 100) {
            slowPrint("恭喜你！積分達到100，遊戲勝利！");
            break;
        }
        slowPrint("你遇到了怪物：" + monster->getName() + "！");
        if (monster->getName() == "哥布林")
            slowPrint("哥布林：我會吃掉你！");
        else if (monster->getName() == "雷射")
            slowPrint("雷射：感受我的雷射光束！");
        else if (monster->getName() == "地精")
            slowPrint("地精：你逃不掉的！");
        else if (monster->getName() == "巨魔")
            slowPrint("巨魔：我會踩扁你！");
        slowPrint("怪物HP：" + std::to_string(monster->getHP()) +
                  " 攻擊力：" + std::to_string(monster->getAttack()));
        battle();
        if (!player->isAlive()) {
            slowPrint("你被擊敗了！最終分數：" + std::to_string(score));
            break;
        }
        std::cout << "選擇行動：1 - 繼續戰鬥, 2 - 進入地圖模式, 3 - 退出遊戲: ";
        int choice;
        std::cin >> choice;
        if (choice == 1) {
            generateMonster();
        } else if (choice == 2) {
            slowPrint("當前血量：" + std::to_string(player->getHP()));
            GameWithMap gameWithMap;
            gameWithMap.start();
        } else if (choice == 3) {
            gameRunning = false;
        } else {
            std::cout << "無效選擇！" << std::endl;
        }
    }
    if (player->isAlive() && score < 100) slowPrint("遊戲結束！最終分數：" + std::to_string(score));
}

void Game::battle() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> crit_dist(1, 10);
    std::vector<std::pair<std::string, float>> moves = {
        {"火焰斬", 1.5},
        {"雷霆一擊", 2.0},
        {"旋風斬", 1.2}
    };
    std::uniform_int_distribution<> move_dist(0, moves.size() - 1);
    std::string celebrates[3] = {"杭特：勝利是我的！", "杭特：下一個是誰？", "杭特：太弱了！"};

    while (player->isAlive() && monster->isAlive()) {
        int moveIndex = move_dist(gen);
        std::string moveName = moves[moveIndex].first;
        float damageMultiplier = moves[moveIndex].second;
        slowPrint("\n杭特使出 " + moveName + " 攻擊 " + monster->getName() + "！");
        int baseDamage = static_cast<int>(player->getAttack() * damageMultiplier);
        int damage = baseDamage;
        if (crit_dist(gen) == 1) {
            damage *= 2;
            slowPrint("爆擊！造成 " + std::to_string(damage) + " 點傷害！");
        } else {
            slowPrint("造成 " + std::to_string(damage) + " 點傷害！");
        }
        monster->takeDamage(damage);
        slowPrint("怪物剩下 HP：" + std::to_string(monster->getHP()));

        if (!monster->isAlive()) {
            int monsterScore = dynamic_cast<Monster*>(monster.get())->getScoreValue();
            score += monsterScore;
            slowPrint(celebrates[move_dist(gen)]);
            slowPrint("\n你打敗了 " + monster->getName() + "！獲得 " + std::to_string(monsterScore) + " 分！");
            Treasure treasure;
            treasure.apply(dynamic_cast<Player*>(player.get()), score);
            break;
        }

        slowPrint(monster->getName() + " 攻擊你！");
        player->takeDamage(monster->getAttack());
        slowPrint("你剩下 HP：" + std::to_string(player->getHP()));

        if (!player->isAlive()) break;

        std::cout << "選擇行動 (attack/retreat): ";
        std::string action;
        std::cin >> action;
        if (action == "retreat") {
            slowPrint("你選擇撤退，返回安全區域。");
            slowPrint("當前血量：" + std::to_string(player->getHP()));
            GameWithMap gameWithMap;
            gameWithMap.start();
            break;
        }
    }
}