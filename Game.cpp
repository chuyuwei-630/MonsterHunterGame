#include "Game.h"
#include <iostream>
#include <random>
#include <memory>

// 定義 Monster 類，繼承自 Character
class Monster : public Character {
public:
    Monster(const std::string& name, int hp, int attack)
        : Character(name, hp, attack) {}
};

// 定義 Player 類，繼承自 Character
class Player : public Character {
public:
    Player(const std::string& name, int hp, int attack)
        : Character(name, hp, attack) {}
};

// Game 類的構造函數
Game::Game() {
    // 初始化隨機數生成器
    std::random_device rd;  // 獲取隨機種子
    std::mt19937 gen(rd()); // Mersenne Twister 隨機數引擎
    std::uniform_int_distribution<> hp_dist(50, 100);      // 怪物 HP 範圍：50-100
    std::uniform_int_distribution<> attack_dist(10, 25);   // 怪物攻擊力範圍：10-25

    // 使用智能指針創建 Player 和 Monster 對象
    player = std::make_unique<Player>("勇者", 100, 25);
    monster = std::make_unique<Monster>("哥布林", hp_dist(gen), attack_dist(gen));
}

// Game 類的析構函數
Game::~Game() {
    // std::unique_ptr 會自動釋放內存，無需手動 delete
}

// 開始遊戲
void Game::start() {
    std::cout << "== 遊戲開始 ==\n";
    std::cout << "你遇到了怪物：" << monster->getName() << "！\n";
    std::cout << "怪物HP：" << monster->getHP() << " 攻擊力：" << monster->getAttack() << "\n";
    battle();
}

// 戰鬥邏輯
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