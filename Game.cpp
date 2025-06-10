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
    void apply(Player* player, int& score) {
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
            std::uniform_int_distribution<> equip_type_dist(1, 4); // WEAPON, ARMOR, ACCESSORY, SHIELD
            int equipType = equip_type_dist(gen);
            
            if (equipType == 1) { // WEAPON
                std::uniform_int_distribution<> weapon_dist(1, 4); // 4 weapon types
                int weaponType = weapon_dist(gen);
                if (weaponType == 1) {
                    auto weapon = std::make_unique<Equipment>("鐵劍", Equipment::WEAPON, 10, 0, 0);
                    player->equip(std::move(weapon));
                    slowPrint("你獲得了一把鐵劍，攻擊力增加 10！");
                } else if (weaponType == 2) {
                    auto staff = std::make_unique<Equipment>("法杖", Equipment::WEAPON, 8, 5, 0);
                    player->equip(std::move(staff));
                    slowPrint("你獲得了一把法杖，攻擊力增加 8，HP增加 5！");
                } else if (weaponType == 3) {
                    auto bow = std::make_unique<Equipment>("弓", Equipment::WEAPON, 12, 0, 0);
                    player->equip(std::move(bow));
                    slowPrint("你獲得了一把弓，攻擊力增加 12！");
                } else {
                    auto axe = std::make_unique<Equipment>("巨斧", Equipment::WEAPON, 15, 0, -2);
                    player->equip(std::move(axe));
                    slowPrint("你獲得了一把巨斧，攻擊力增加 15，防禦力減少 2！");
                }
            } else if (equipType == 2) { // ARMOR
                std::uniform_int_distribution<> armor_dist(1, 4); // 4 armor types
                int armorType = armor_dist(gen);
                if (armorType == 1) {
                    auto armor = std::make_unique<Equipment>("皮甲", Equipment::ARMOR, 0, 10, 5);
                    player->equip(std::move(armor));
                    slowPrint("你獲得了一件皮甲，HP增加 10，防禦力增加 5！");
                } else if (armorType == 2) {
                    auto cloth = std::make_unique<Equipment>("布甲", Equipment::ARMOR, 0, 8, 3);
                    player->equip(std::move(cloth));
                    slowPrint("你獲得了一件布甲，HP增加 8，防禦力增加 3！");
                } else if (armorType == 3) {
                    auto iron = std::make_unique<Equipment>("鐵甲", Equipment::ARMOR, 0, 12, 8);
                    player->equip(std::move(iron));
                    slowPrint("你獲得了一件鐵甲，HP增加 12，防禦力增加 8！");
                } else {
                    auto copper = std::make_unique<Equipment>("銅甲", Equipment::ARMOR, 0, 10, 6);
                    player->equip(std::move(copper));
                    slowPrint("你獲得了一件銅甲，HP增加 10，防禦力增加 6！");
                }
            } else if (equipType == 3) { // ACCESSORY
                std::uniform_int_distribution<> accessory_dist(1, 3); // 3 accessory types
                int accessoryType = accessory_dist(gen);
                if (accessoryType == 1) {
                    auto accessory = std::make_unique<Equipment>("力量戒指", Equipment::ACCESSORY, 5, 5, 0);
                    player->equip(std::move(accessory));
                    slowPrint("你獲得了一枚力量戒指，攻擊力增加 5，HP增加 5！");
                } else if (accessoryType == 2) {
                    auto lifeRing = std::make_unique<Equipment>("生命戒指", Equipment::ACCESSORY, 0, 10, 0);
                    player->equip(std::move(lifeRing));
                    slowPrint("你獲得了一枚生命戒指，HP增加 10！");
                } else {
                    auto defRing = std::make_unique<Equipment>("防禦戒指", Equipment::ACCESSORY, 0, 0, 5);
                    player->equip(std::move(defRing));
                    slowPrint("你獲得了一枚防禦戒指，防禦力增加 5！");
                }
            } else { // SHIELD
                std::uniform_int_distribution<> shield_dist(1, 4); // 4 shield types
                int shieldType = shield_dist(gen);
                if (shieldType == 1) {
                    auto shield = std::make_unique<Equipment>("木盾", Equipment::SHIELD, 0, 5, 10);
                    player->equip(std::move(shield));
                    slowPrint("你獲得了一面木盾，HP增加 5，防禦力增加 10！");
                } else if (shieldType == 2) {
                    auto hardShield = std::make_unique<Equipment>("硬木盾", Equipment::SHIELD, 0, 6, 12);
                    player->equip(std::move(hardShield));
                    slowPrint("你獲得了一面硬木盾，HP增加 6，防禦力增加 12！");
                } else if (shieldType == 3) {
                    auto ironShield = std::make_unique<Equipment>("鐵盾", Equipment::SHIELD, 0, 8, 15);
                    player->equip(std::move(ironShield));
                    slowPrint("你獲得了一面鐵盾，HP增加 8，防禦力增加 15！");
                } else {
                    auto magicShield = std::make_unique<Equipment>("法術盾", Equipment::SHIELD, 2, 5, 10);
                    player->equip(std::move(magicShield));
                    slowPrint("你獲得了一面法術盾，攻擊力增加 2，HP增加 5，防禦力增加 10！");
                }
            }
        }
    }
private:
    int type;
    int value;
};

Game::Game() {
    player = std::make_unique<Player>("杭特", 100, 25, 0);
    score = 0;
    startTime = std::chrono::steady_clock::now();
    generateMonster();
}

Game::~Game() {}

void Game::generateMonster() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> hp_dist(50, 100);
    std::uniform_int_distribution<> attack_dist(10, 25);
    std::uniform_int_distribution<> defense_dist(0, 5);
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
ічні    std::string celebrates[3] = {"杭特：勝利是我的！", "杭特：下一個是誰？", "杭特：太弱了！"};

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
        slowPrint("脖子剩下 HP：" + std::to_string(player->getHP()));

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
