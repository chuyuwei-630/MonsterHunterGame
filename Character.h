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
    int defense;
    int hpRegen;
    std::map<Equipment::Type, std::unique_ptr<Equipment>> equippedItems;
    std::vector<std::unique_ptr<Equipment>> inventory;

public:
    Character(const std::string& name, int hp, int attack, int defense, int hpRegen = 0);
    virtual ~Character() = default;

    void equip(std::unique_ptr<Equipment> item);
    void unequip(Equipment::Type type);
    void addToInventory(std::unique_ptr<Equipment> item);
    int getHpRegen() const;
    std::string getName() const;
};

#endif // CHARACTER_H



