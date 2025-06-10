#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include "Equipment.h"
#include <memory>
#include <map>
#include <vector>

class Character {
protected:
    std::string name;
    int hp;
    int attack;
    int defense; // 新增防禦力屬性
    std::map<Equipment::Type, std::unique_ptr<Equipment>> equippedItems; // 多個裝備槽
    std::vector<std::unique_ptr<Equipment>> inventory; // 裝備庫存

public:
    Character(const std::string& name, int hp, int attack, int defense = 0);
    virtual ~Character() = default;

    void equip(std::unique_ptr<Equipment> item);
    void unequip(Equipment::Type type);
    void addToInventory(std::unique_ptr<Equipment> item);
    void equipFromInventory(size_t index);
    void takeDamage(int dmg);
    bool isAlive() const;
    int getAttack() const;
    int getHP() const;
    int getDefense() const; // 新增獲取防禦力的方法
    std::string getName() const;
    void increaseAttack(int amount);
    void displayInventory() const; // 查看庫存
};

#endif //CHARACTER_H
