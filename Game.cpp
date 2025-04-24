#include "Game.h"
#include <iostream>
#include <random>

// 子類別定義
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

void Game::start() {
    std::cout << "== 遊戲開始 ==\n";
    std::cout << "你遇到了怪物：" << monster->getName() << "！\n";
    std::cout << "怪物HP：" << monster->getHP() << " 攻擊力：" << monster->getAttack() << "\n";
    battle();
}

void Game::battle() {
    while (player->isAlive() && monster->isAlive()) {
        std::cout << "\n你攻擊了 " << monster->getName() << "！\n";
        monster->takeDamage(player->getAttack());
        std::cout << "怪物剩下 HP：" << monster->getHP() << "\n";

        if (!monster->isAlive()) break;

        std::cout << monster->getName() << " 反擊你！\n";
        player->takeDamage(monster->getAttack());
        std::cout << "你剩下 HP：" << player->getHP() << "\n";
    }

    if (player->isAlive())
        std::cout << "\n你打敗了怪物！勝利！\n";
    else
        std::cout << "\n你被怪物擊敗了...\n";
}
