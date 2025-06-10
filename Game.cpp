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

class Treasure {
public:
    Treasure() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> type_dist(1, 4);
        type = type_dist(gen);
        if (type == 1) value = 20; // Health
        else if (type == 2) value = 5; // Attack Boost
        else if (type == 3) value = 10; // Score (Experience)
        else {
            std::uniform_int_distribution<> rarity_chance(1, 100);
            int chance = rarity_chance(gen);
            if (chance <= 60) rarity = Equipment::COMMON;
            else if (chance <= 85) rarity = Equipment::RARE;
            else if (chance <= 95) rarity = Equipment::EPIC;
            else rarity = Equipment::LEGENDARY;
        }
    }
    void apply(Player* player, int& score) {
        std::random_device rd;
        std::mt19937 gen(rd());
        if (type == 1) {
            player->heal(value);
            slowPrint("你獲得回血寶箱，回復 " + std::to_string(value) + " HP！");
        } else if (type == 2) {
            player->increaseAttack(value);
            slowPrint("你獲得了攻擊力寶箱，攻擊力增加 " + std::to_string(value) + "！");
        } else if (type == 3) {
            score += value;
            slowPrint("你獲得了經驗寶箱，增加 " + std::to_string(value) + " 分！");
        } else {
            std::uniform_int_distribution<> equip_type_dist(1, 5);
            int equipType = equip_type_dist(gen);
            float rarityMultiplier = (rarity == Equipment::COMMON) ? 1.0f :
                                    (rarity == Equipment::RARE) ? 1.5f :
                                    (rarity == Equipment::EPIC) ? 2.0f : 2.5f;

            if (equipType == 1) {
                std::uniform_int_distribution<> weapon_dist(1, 4);
                int weaponType = weapon_dist(gen);
                if (weaponType == 1) {
                    int attack = static_cast<int>(10 * rarityMultiplier);
                    auto weapon = std::make_unique<Equipment>("鐵劍", Equipment::WEAPON, attack, 0, 0, rarity, Element::NONE);
                    player->addToInventory(std::move(weapon));
                    slowPrint("你獲得了一把[" + weapon->getRarityString() + "]鐵劍，攻擊力增加 " + std::to_string(attack) + "！已加入庫存。");
                } else if (weaponType == 2) {
                    int attack = static_cast<int>(8 * rarityMultiplier);
                    auto weapon = std::make_unique<Equipment>("火劍", Equipment::WEAPON, attack, 0, 0, rarity, Element::FIRE);
                    player->addToInventory(std::move(weapon));
                    slowPrint("你獲得了一把[" + weapon->getRarityString() + "]火劍，攻擊力增加 " + std::to_string(attack) + "！已加入庫存。");
                } else if (weaponType == 3) {
                    int attack = static_cast<int>(12 * rarityMultiplier);
                    auto weapon = std::make_unique<Equipment>("風弓", Equipment::WEAPON, attack, 0, 0, rarity, Element::AIR);
                    player->addToInventory(std::move(weapon));
                    slowPrint("你獲得了一把[" + weapon->getRarityString() + "]風弓，攻擊力增加 " + std::to_string(attack) + "！已加入庫存。");
                } else {
                    int attack = static_cast<int>(15 * rarityMultiplier);
                    auto weapon = std::make_unique<Equipment>("雷斧", Equipment::WEAPON, attack, 0, 0, rarity, Element::AIR);
                    player->addToInventory(std::move(weapon));
                    slowPrint("你獲得了一把[" + weapon->getRarityString() + "]雷斧，攻擊力增加 " + std::to_string(attack) + "！已加入庫存。");
                }
            } else if (equipType == 2) {
                int hp = static_cast<int>(20 * rarityMultiplier);
                int def = static_cast<int>(5 * rarityMultiplier);
                auto armor = std::make_unique<Equipment>("盔甲", Equipment::ARMOR, 0, hp, def, rarity);
                player->addToInventory(std::move(armor));
                slowPrint("你獲得了一件[" + armor->getRarityString() + "]盔甲，HP增加 " + std::to_string(hp) + "，防禦力增加 " + std::to_string(def) + "！已加入庫存。");
            } else if (equipType == 3) {
                int attack = static_cast<int>(5 * rarityMultiplier);
                int hp = static_cast<int>(10 * rarityMultiplier);
                auto accessory = std::make_unique<Equipment>("戒指", Equipment::ACCESSORY, attack, hp, 0, rarity);
                player->addToInventory(std::move(accessory));
                slowPrint("你獲得了一枚[" + accessory->getRarityString() + "]戒指，攻擊力增加 " + std::to_string(attack) + "，HP增加 " + std::to_string(hp) + "！已加入庫存。");
            } else if (equipType == 4) {
                int hp = static_cast<int>(5 * rarityMultiplier);
                int def = static_cast<int>(10 * rarityMultiplier);
                auto shield = std::make_unique<Equipment>("盾牌", Equipment::SHIELD, 0, hp, def, rarity);
                player->addToInventory(std::move(shield));
                slowPrint("你獲得了一面[" + shield->getRarityString() + "]盾牌，HP增加 " + std::to_string(hp) + "，防禦力增加 " + std::to_string(def) + "！已加入庫存。");
            } else {
                int attack = static_cast<int>(10 * rarityMultiplier);
                auto gloves = std::make_unique<Equipment>("手套", Equipment::GLOVES, attack, 0, 0, rarity);
                player->addToInventory(std::move(gloves));
                slowPrint("你獲得了一雙[" + gloves->getRarityString() + "]手套，攻擊力增加 " + std::to_string(attack) + "！已加入庫存。");
            }
        }
    }
private:
    int type;
    int value;
    Equipment::Rarity rarity;
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

int showOptions() {
    std::cout << "\n選擇行動：\n1 - 繼續戰鬥\n2 - 進入地圖模式\n3 - 查看並管理庫存\n4 - 退出遊戲\n你的選擇: ";
    int choice;
    std::cin >> choice;
    return choice;
}

void Game::start() {
    slowPrint("== 遊戲開始 ==");
    bool gameRunning = true;
    int monstersDefeated = 0;

    // 遊戲開始時進入地圖模式
    slowPrint("當前血量：" + std::to_string(player->getHP()));
    GameWithMap gameWithMap(*player);
    gameWithMap.start();

    while (gameRunning && player->isAlive()) {
        if (monstersDefeated >= 10) {
            auto endTime = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();
            slowPrint("恭喜你！打敗了10隻怪物，遊戲勝利！遊戲時間：" + std::to_string(duration) + " 秒");
            break;
        }

        int choice = showOptions();
        switch (choice) {
            case 1:
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
                if (!monster->isAlive()) {
                    monstersDefeated++;
                }
                break;
            case 2: {
                slowPrint("當前血量：" + std::to_string(player->getHP()));
                GameWithMap gameWithMap(*player);
                gameWithMap.start();
                break;
            }
            case 3: {
                player->displayInventory();
                std::cout << "輸入要裝備的庫存編號（或 -1 取消）：";
                int index;
                std::cin >> index;
                if (index >= 0) {
                    player->equipFromInventory(index);
                    slowPrint("已裝備選定的裝備！");
                }
                break;
            }
            case 4:
                gameRunning = false;
                break;
            default:
                std::cout << "無效選擇！" << std::endl;
                break;
        }
    }
    if (player->isAlive() && monstersDefeated < 10) {
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
        {"普通攻擊", 1.0},
        {"火焰斬", 1.5},
        {"雷霆一擊", 2.0},
        {"旋風斬", 1.2}
    };
    std::string celebrates[3] = {"杭特：勝利是我的！", "杭特：下一個是誰？", "杭特：太弱了！"};

    while (player->isAlive() && monster->isAlive()) {
        std::cout << "選擇招式：" << std::endl;
        for (size_t i = 0; i < moves.size(); ++i) {
            std::cout << i + 1 << ": " << moves[i].first << std::endl;
        }
        int choice;
        std::cin >> choice;
        if (choice < 1 || choice > static_cast<int>(moves.size())) {
            std::cout << "無效選擇！使用普通攻擊。" << std::endl;
            choice = 1;
        }
        std::string moveName = moves[choice - 1].first;
        float damageMultiplier = moves[choice - 1].second;

        Element weaponElement = dynamic_cast<Player*>(player.get())->getEquippedWeaponElement();
        if (moveName == "火焰斬" && weaponElement == Element::FIRE) {
            damageMultiplier *= 1.2;
            slowPrint("火劍與火焰斬搭配，傷害提升！");
        } else if (moveName == "雷霆一擊" && weaponElement == Element::AIR) {
            damageMultiplier *= 1.2;
            slowPrint("雷斧與雷霆一擊搭配，傷害提升！");
        } else if (moveName == "旋風斬" && weaponElement == Element::AIR) {
            damageMultiplier *= 1.2;
            slowPrint("風弓與旋風斬搭配，傷害提升！");
        }

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
            slowPrint(celebrates[std::uniform_int_distribution<>(0, 2)(gen)]);
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
            GameWithMap gameWithMap(*player);
            gameWithMap.start();
            break;
        }
    }
}