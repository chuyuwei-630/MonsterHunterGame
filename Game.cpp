#include "Game.h"
#include <iostream>
#include <random>

class Monster : public Character {
public:
    Monster(const std::string& name, int hp, int attack)
        : Character(name, hp, attack) {}
};

class Player : public Character {
public:
    Player(const std::string& name, int hp, int attack)
        : Character(name, hp, attack) {}
};

Game::Game() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> hp_dist(50, 100);
    std::uniform_int_distribution<> attack_dist(10, 25);

    player = std::make_unique<Player>("勇者", 100, 25);
    monster = std::make_unique<Monster>("哥布林", hp_dist(gen), attack_dist(gen));
}

Game::~Game() {}

#include <thread>   // 加這行 for sleep
#include <chrono>   // for chrono::milliseconds

void slowPrint(const std::string& text, int delay = 50) {
    for (char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
    std::cout << std::endl;
}

void Game::start() {
    slowPrint("== 遊戲開始 ==");
    slowPrint("你遇到了怪物：" + monster->getName() + "！");
    slowPrint("怪物HP：" + std::to_string(monster->getHP()) +
              " 攻擊力：" + std::to_string(monster->getAttack()));
    battle();
}

void Game::battle() {
    while (player->isAlive() && monster->isAlive()) {
        slowPrint("\n你揮劍攻擊 " + monster->getName() + "！");
        monster->takeDamage(player->getAttack());
        slowPrint("怪物剩下 HP：" + std::to_string(monster->getHP()));

        if (!monster->isAlive()) break;

        slowPrint(monster->getName() + " 痾啊啊啊啊！");
        player->takeDamage(monster->getAttack());
        slowPrint("你剩下 HP：" + std::to_string(player->getHP()));
    }

    if (player->isAlive())
        slowPrint("\n你打敗了怪物！勝利！");
    else
        slowPrint("\n你被怪物擊敗了...");
}

