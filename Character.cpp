#include "Character.h"
#include <iostream>

Character::Character(const std::string& name, int hp, int attack, int defense)
    : name(name), hp(hp), attack(attack), defense(defense) {}

void Character::equip(std::unique_ptr<Equipment> item) {
    Equipment::Type type = item->getType();
    if (equippedItems.find(type) != equippedItems.end()) {
        unequip(type); // 如果該槽位已有裝備，先卸下
    }
    equippedItems[type] = std::move(item);
    attack += equippedItems[type]->getAttackBonus();
    hp += equippedItems[type]->getHpBonus();
    defense += equippedItems[type]->getDefenseBonus();
}

void Character::unequip(Equipment::Type type) {
    if (equippedItems.find(type) != equippedItems.end()) {
        attack -= equippedItems[type]->getAttackBonus();
        hp -= equippedItems[type]->getHpBonus();
        defense -= equippedItems[type]->getDefenseBonus();
        addToInventory(std::move(equippedItems[type])); // 卸下的裝備放入庫存
        equippedItems.erase(type);
    }
}

void Character::addToInventory(std::unique_ptr<Equipment> item) {
    inventory.push_back(std::move(item));
}

void Character::equipFromInventory(size_t index) {
    if (index < inventory.size()) {
        equip(std::move(inventory[index]));
        inventory.erase(inventory.begin() + index);
    } else {
        std::cout << "無效的庫存索引！" << std::endl;
    }
}

void Character::takeDamage(int dmg) {
    int actualDamage = dmg - defense; // 考慮防禦力減傷
    if (actualDamage < 0) actualDamage = 0;
    hp -= actualDamage;
    if (hp < 0) hp = 0;
}

bool Character::isAlive() const {
    return hp > 0;
}

int Character::getAttack() const {
    return attack;
}

int Character::getHP() const {
    return hp;
}

int Character::getDefense() const {
    return defense;
}

std::string Character::getName() const {
    return name;
}

void Character::increaseAttack(int amount) {
    attack += amount;
}

void Character::displayInventory() const {
    std::cout << "庫存中的裝備：" << std::endl;
    if (inventory.empty()) {
        std::cout << "（空）" << std::endl;
    } else {
        for (size_t i = 0; i < inventory.size(); ++i) {
            std::cout << i << ": " << inventory[i]->getName() 
                      << " (攻擊+" << inventory[i]->getAttackBonus() 
                      << ", HP+" << inventory[i]->getHpBonus() 
                      << ", 防禦+" << inventory[i]->getDefenseBonus() << ")" << std::endl;
        }
    }
}
