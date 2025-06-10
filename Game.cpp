#include "Game.h"
#include "GameWithMap.h"
#include <iostream>
#include <random>
#include <thread>
#include <chrono>
#include <vector>

class Monster : public Character {
public:
    Monster(const std::string& name, int hp, int attack, int defense, int scoreValue)
        : Character(name, hp, attack, defense), scoreValue(scoreValue) {}
    int getScoreValue() const { return scoreValue; }
private:
    int scoreValue;
};

class Player : public Character {
public:
    Player(const std::string& name, int hp, int attack, int defense)
        : Character(name, hp, attack, defense) {}
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
        std::uniform_int_distribution<> type_dist(1, 4);
        type = type_dist(gen);
        if (type == 1) value = 20; // 回血
        else if (type == 2) value = 5; // 攻擊力
        else if (type == 3) value = 10; // 積分
        else value = 4; // 裝備
    }
  void Treasure::apply(Player* player, int& score) {
    if (type == 1) {
        player->heal(value);
        slowPrint("你獲得回血寶箱，回復 " + std::to_string(value) + " HP！");
    } else if (type == 2) {
        player->increaseAttack(value);
        slowPrint("你獲得了攻擊力寶箱，攻擊力增加 " + std::to_string(value) + "！");
    } else if (type == 3) {
        score += value;
        slowPrint("你獲得了積分寶箱，增加 " + std::to_string(value) + " 積分！");
    } else if (type == 4) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> equip_dist(1, 4);
        int equipType = equip_dist(gen);
        if (equipType == 1) {
            auto sword = std::make_unique<Equipment>("長劍", Equipment::WEAPON, 10, 10, 10, 10);
            player->equip(std::move(sword));
            slowPrint("你獲得了一把長劍，攻擊力+10，HP+10，防禦+10，回血+10！");
        } else if (equipType == 2) {
            auto bow = std::make_unique<Equipment>("弓箭", Equipment::WEAPON, 15, 5, 5, 5);
            player->equip(std::move(bow));
            slowPrint("你獲得了一把弓箭，攻擊力+15，HP+5，防禦+5，回血+5！");
        } else if (equipType == 3) {
            auto staff = std::make_unique<Equipment>("法杖", Equipment::WEAPON, 12, 8, 8, 5);
            player->equip(std::move(staff));
            slowPrint("你獲得了一把法杖，攻擊力+12，HP+8，防禦+8，回血+5！");
        } else {
            auto shield = std::make_unique<Equipment>("盾牌", Equipment::ARMOR, 3, 15, 15, 10);
            player->equip(std::move(shield));
            slowPrint("你獲得了一面盾牌，攻擊力+3，HP+15，防禦+15，回血+10！");
        }
    }
}
private:
    int type;
    int value;
};

Game::Game() {
    player = std::make_unique<Player>("杭特", 100, 25, 0); // 初始防禦力為 0
    score = 0;
    startTime = std::chrono::steady_clock::now(); // 記錄開始時間
    generateMonster();
}

Game::~Game() {}

void Game::generateMonster() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> hp_dist(50, 100);
    std::uniform_int_distribution<> attack_dist(10, 25);
    std::uniform_int_distribution<> defense_dist(0, 5); // 怪物防禦力
    std::uniform_int_distribution<> monster_dist(1, 4);

    int hp = hp_dist(gen);
    int atk = attack_dist(gen);
    int def = defense_dist(gen);
    int monsterType = monster_dist(gen);

    if (monsterType == 1)
        monster = std::make_unique<Monster>("哥布林", hp, atk, def, 10);
    else if (monsterType == 2)
        monster = std::make_unique<Monster>("雷射", hp, atk + 5, def, 15);
    else if (monsterType == 3)
        monster = std::make_unique<Monster>("地精", hp, atk, def, 12);
    else
        monster = std::make_unique<Monster>("巨魔", hp + 20, atk + 10, def + 5, 20);
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
            auto endTime = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();
            slowPrint("恭喜你！積分達到100，遊戲勝利！遊戲時間：" + std::to_string(duration) + " 秒");
            break;
        }

        if (!monster || !monster->isAlive()) {
            generateMonster();
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
                  " 攻擊力：" + std::to_string(monster->getAttack()) +
                  " 防禦力：" + std::to_string(monster->getDefense()));
        battle();
        if (!player->isAlive()) {
            auto endTime = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();
            slowPrint("你被擊敗了！最終分數：" + std::to_string(score) + "，遊戲時間：" + std::to_string(duration) + " 秒");
            break;
        }
        std::cout << "選擇行動：1 - 繼續戰鬥, 2 - 進入地圖模式, 3 - 退出遊戲, 4 - 查看並管理庫存: ";
        int choice;
        std::cin >> choice;
        if (choice == 1) {
            generateMonster();
        } else if (choice == 2) {
            slowPrint("當前血量：" + std::to_string(player->getHP()));
            GameWithMap gameWithMap;
            gameWithMap.start();
            generateMonster();
        } else if (choice == 3) {
            gameRunning = false;
        } else if (choice == 4) {
            player->displayInventory();
            std::cout << "輸入要裝備的庫存編號（或 -1 取消）：";
            int index;
            std::cin >> index;
            if (index >= 0) {
                player->equipFromInventory(index);
                slowPrint("已裝備選定的裝備！");
            }
        } else {
            std::cout << "無效選擇！" << std::endl;
        }
    }
    if (player->isAlive() && score < 100) {
        auto endTime = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();
        slowPrint("遊戲結束！最終分數：" + std::to_string(score) + "，遊戲時間：" + std::to_string(duration) + " 秒");
    }
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
            generateMonster();
            break;
        }
    }
}

