#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include "Equipment.h"
#include <memory>

class Character {
protected:
    std::string name;
    int hp;
    int attack;
    std::unique_ptr<Equipment> equippedItem;

public:
    Character(const std::string& name, int hp, int attack);
    virtual ~Character() = default;

    void equip(std::unique_ptr<Equipment> item);
    void unequip();
    void takeDamage(int dmg);
    bool isAlive() const;
    int getAttack() const;
    int getHP() const;
    std::string getName() const;
    void increaseAttack(int amount);
};

#endif // CHARACTER_H



