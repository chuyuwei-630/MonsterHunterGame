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
        std::uniform_int_distribution<> type_dist(1, 100);
        int roll = type_dist(gen);
        if (roll <= 15) {
            type = 1;
            value = 20;
        } else if (roll <= 30) {
            type = 2;
            value = 5;
        } else if (roll <= 45) {
            type = 3;
            value = 10;
        } else {
            type = 4;
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
                                     (rarity == Equipment::RARE) ? 1.2f :
                                     (rarity == Equipment::EPIC) ? 1.5f : 1.8f;

            if (equipType == 1) {
                std::vector<std::tuple<std::string, int, int, int, Element>> weapons = {
                    {"鐵劍", 7, 0, 0, Element::NONE},
                    {"法杖", 5, 2, 0, Element::FIRE},
                    {"弓", 9, 0, -1, Element::AIR},
                    {"巨斧", 12, 1, -3, Element::AIR}
                };
                std::uniform_int_distribution<> weapon_index(0, weapons.size() - 1);
                int idx = weapon_index(gen);
                auto [name, baseAtk, baseHp, baseDef, element] = weapons[idx];
                int atk = static_cast<int>(baseAtk * rarityMultiplier);
                int hp = static_cast<int>(baseHp * rarityMultiplier);
                int def = static_cast<int>(baseDef * rarityMultiplier);
                auto weapon = std::make_unique<Equipment>(name, Equipment::WEAPON, atk, hp, def, rarity, element);
                std::string rarityStr = weapon->getRarityString();
                player->addToInventory(std::move(weapon));
                std::string defStr = (def >= 0) ? "增加 " + std::to_string(def) : "減少 " + std::to_string(-def);
                std::string message = "你獲得了一把[" + rarityStr + "]" + name + "，攻擊力增加 " + std::to_string(atk);
                if (hp != 0) {
                    message += "，HP";
                    message += (hp >= 0 ? "增加 " : "減少 ");
                    message += std::to_string(std::abs(hp));
                }
                message += "，防禦力" + defStr + "！已加入庫存。";
                if (rarity == Equipment::LEGENDARY) message += "（傳說效果：死亡時復活一次，HP恢復至1）";
                slowPrint(message);
            } else if (equipType == 2) {
                std::vector<std::tuple<std::string, int, int, int>> armors = {
                    {"布甲", 0, 5, 2},
                    {"皮甲", 0, 10, 3},
                    {"銅甲", 0, 15, 4},
                    {"鐵甲", 0, 20, 5}
                };
                std::uniform_int_distribution<> armor_index(0, armors.size() - 1);
                int idx = armor_index(gen);
                auto [name, baseAtk, baseHp, baseDef] = armors[idx];
                int atk = static_cast<int>(baseAtk * rarityMultiplier);
                int hp = static_cast<int>(baseHp * rarityMultiplier);
                int def = static_cast<int>(baseDef * rarityMultiplier);
                auto armor = std::make_unique<Equipment>(name, Equipment::ARMOR, atk, hp, def, rarity);
                std::string rarityStr = armor->getRarityString();
                player->addToInventory(std::move(armor));
                std::string message = "你獲得了一件[" + rarityStr + "]" + name + "，HP增加 " + std::to_string(hp) + "，防禦力增加 " + std::to_string(def) + "！已加入庫存。";
                if (rarity == Equipment::LEGENDARY) message += "（傳說效果：反彈25%受到的傷害）";
                slowPrint(message);
            } else if (equipType == 3) {
                std::vector<std::tuple<std::string, int, int, int>> accessories = {
                    {"力量戒指", 5, 0, 0},
                    {"生命戒指", 0, 10, 0},
                    {"防禦戒指", 0, 0, 3}
                };
                std::uniform_int_distribution<> accessory_index(0, accessories.size() - 1);
                int idx = accessory_index(gen);
                auto [name, baseAtk, baseHp, baseDef] = accessories[idx];
                int atk = static_cast<int>(baseAtk * rarityMultiplier);
                int hp = static_cast<int>(baseHp * rarityMultiplier);
                int def = static_cast<int>(baseDef * rarityMultiplier);
                auto accessory = std::make_unique<Equipment>(name, Equipment::ACCESSORY, atk, hp, def, rarity);
                std::string rarityStr = accessory->getRarityString();
                player->addToInventory(std::move(accessory));
                std::string message = "你獲得了一枚[" + rarityStr + "]" + name + "！";
                if (atk > 0) message += "攻擊力增加 " + std::to_string(atk) + "，";
                if (hp > 0) message += "HP增加 " + std::to_string(hp) + "，";
                if (def > 0) message += "防禦力增加 " + std::to_string(def) + "，";
                message += "已加入庫存。";
                if (rarity == Equipment::LEGENDARY) message += "（傳說效果：所有屬性增加10%）";
                slowPrint(message);
            } else if (equipType == 4) {
                std::vector<std::tuple<std::string, int, int, int>> shields = {
                    {"木盾", 0, 5, 5},
                    {"硬木盾", 0, 7, 7},
                    {"鐵盾", 0, 10, 10},
                    {"法術盾", 4, 4, 4}
                };
                std::uniform_int_distribution<> shield_index(0, shields.size() - 1);
                int idx = shield_index(gen);
                auto [name, baseAtk, baseHp, baseDef] = shields[idx];
                int atk = static_cast<int>(baseAtk * rarityMultiplier);
                int hp = static_cast<int>(baseHp * rarityMultiplier);
                int def = static_cast<int>(baseDef * rarityMultiplier);
                auto shield = std::make_unique<Equipment>(name, Equipment::SHIELD, atk, hp, def, rarity);
                std::string rarityStr = shield->getRarityString();
                player->addToInventory(std::move(shield));
                std::string message = "你獲得了一面[" + rarityStr + "]" + name + "！";
                if (atk > 0) message += "攻擊力增加 " + std::to_string(atk) + "，";
                if (hp > 0) message += "HP增加 " + std::to_string(hp) + "，";
                if (def > 0) message += "防禦力增加 " + std::to_string(def) + "，";
                message += "已加入庫存。";
                if (rarity == Equipment::LEGENDARY) message += "（傳說效果：25%閃避機率）";
                slowPrint(message);
            } else if (equipType == 5) {
                std::vector<std::tuple<std::string, int, int, int>> gloves = {
                    {"拳擊手套", 7, 0, 0},
                    {"魔力手套", 5, 3, 0},
                    {"敏捷手套", 5, 0, 3},
                    {"全能手套", 3, 3, 3}
                };
                std::uniform_int_distribution<> glove_index(0, gloves.size() - 1);
                int idx = glove_index(gen);
                auto [name, baseAtk, baseHp, baseDef] = gloves[idx];
                int atk = static_cast<int>(baseAtk * rarityMultiplier);
                int hp = static_cast<int>(baseHp * rarityMultiplier);
                int def = static_cast<int>(baseDef * rarityMultiplier);
                auto glove = std::make_unique<Equipment>(name, Equipment::GLOVES, atk, hp, def, rarity);
                std::string rarityStr = glove->getRarityString();
                player->addToInventory(std::move(glove));
                std::string message = "你獲得了一雙[" + rarityStr + "]" + name + "！";
                if (atk > 0) message += "攻擊力增加 " + std::to_string(atk) + "，";
                if (hp > 0) message += "HP增加 " + std::to_string(hp) + "，";
                if (def > 0) message += "防禦力增加 " + std::to_string(def) + "，";
                message += "已加入庫存。";
                if (rarity == Equipment::LEGENDARY) message += "（傳說效果：額外增加5攻擊力）";
                slowPrint(message);
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
    mapCompleted = true;
    evolutionActive = false;
    monstersDefeated = 0; // 初始化打敗怪物數量
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

void Game::applyDestinyStone() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<std::tuple<std::string, std::vector<std::string>, int>> questions = {
        {"以下哪個是杭特的初始武器？", {"鐵劍", "法杖", "弓"}, 0},
        {"哥布林的典型弱點是什麼？", {"火", "水", "風"}, 0},
        {"傳說武器的特殊效果是什麼？", {"復活", "反彈傷害", "閃避"}, 0}
    };
    std::uniform_int_distribution<> q_dist(0, questions.size() - 1);
    int q_idx = q_dist(gen);
    auto [question, options, correct] = questions[q_idx];

    slowPrint("命運石發動！回答以下問題：");
    slowPrint(question);
    for (size_t i = 0; i < options.size(); ++i) {
        std::cout << i + 1 << ": " << options[i] << std::endl;
    }
    std::cout << "你的答案（1-3）：";
    int answer;
    std::cin >> answer;
    if (answer - 1 == correct) {
        player->heal(30);
        slowPrint("回答正確！回復30 HP！");
    } else {
        slowPrint("回答錯誤，沒有獎勵。");
    }
}

void Game::resetEvolutionBoost() {
    if (evolutionActive) {
        player->resetEvolutionBoost();
        evolutionActive = false;
        slowPrint("進化石效果結束，屬性恢復正常。");
    }
}

int showOptions(bool mapCompleted, int totalStonesCollected) {
    std::cout << "\n選擇行動：\n1 - 繼續戰鬥\n";
    if (totalStonesCollected >= 25) {
        std::cout << "2 - 地圖模式（已收集25顆寶石，無法再次進入）\n";
    } else if (!mapCompleted) {
        std::cout << "2 - 進入地圖模式（已完成本輪地圖）\n";
    } else {
        std::cout << "2 - 進入地圖模式\n";
    }
    std::cout << "3 - 查看並管理庫存\n4 - 退出遊戲\n你的選擇: ";
    int choice;
    std::cin >> choice;
    return choice;
}

void Game::start() {
    slowPrint("== 遊戲開始 ==");
    bool gameRunning = true;
    int totalStonesCollected = 0;

    while (gameRunning && player->isAlive()) {
        if (monstersDefeated >= 10) {
            auto endTime = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();
            slowPrint("恭喜你！打敗了10隻怪物，遊戲勝利！最終分數：" + std::to_string(score) + "，遊戲時間：" + std::to_string(duration) + " 秒");
            break;
        }

        int choice = showOptions(mapCompleted, totalStonesCollected);
        switch (choice) {
            case 1:
                if (!monster || !monster->isAlive()) {
                    resetEvolutionBoost();
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
                break;
            case 2: {
                if (totalStonesCollected >= 25) {
                    slowPrint("你已收集25顆寶石，無法再次進入地圖！");
                    break;
                }
                if (mapCompleted) {
                    slowPrint("當前血量：" + std::to_string(player->getHP()));
                    GameWithMap gameWithMap(*player);
                    gameWithMap.start(false);
                    totalStonesCollected = gameWithMap.getTotalStonesCollected();
                    if (!player->isAlive()) {
                        auto endTime = std::chrono::steady_clock::now();
                        auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();
                        slowPrint("你在地圖中死亡！最終分數：" + std::to_string(score) + "，遊戲時間：" + std::to_string(duration) + " 秒");
                        gameRunning = false;
                    }
                    mapCompleted = gameWithMap.hasCollectedTwoStones();
                } else {
                    slowPrint("你已完成本輪地圖，請繼續戰鬥！");
                }
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
    bool repairUsed = false;

    while (player->isAlive() && monster->isAlive()) {
        std::cout << "選擇招式：" << std::endl;
        for (size_t i = 0; i < moves.size(); ++i) {
            std::cout << i + 1 << ": " << moves[i].first << std::endl;
        }
        if (!repairUsed && player->hasRepairStone) {
            std::cout << moves.size() + 1 << ": 使用修復石修復武器" << std::endl;
        }
        std::cout << "你的選擇：";
        int choice;
        std::cin >> choice;

        if (!repairUsed && player->hasRepairStone && choice == static_cast<int>(moves.size()) + 1) {
            player->repairWeapon();
            repairUsed = true;
            slowPrint("你使用了修復石，武器已修復！");
            continue;
        }

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
            monstersDefeated++; // 增加打敗的怪物數量
            Treasure treasure;
            treasure.apply(dynamic_cast<Player*>(player.get()), score);
            resetEvolutionBoost();
            break;
        }

        slowPrint(monster->getName() + " 攻擊你！");
        player->takeDamage(monster->getAttack(), monster.get());
        slowPrint("你剩下 HP：" + std::to_string(player->getHP()));

        if (!player->isAlive()) break;

        std::cout << "選擇行動 (attack/retreat): ";
        std::string action;
        std::cin >> action;
        if (action == "retreat") {
            slowPrint("你選擇撤退，返回安全區域。");
            break;
        }
    }
}