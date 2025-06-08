#include "Character.h"

Character::Character(const std::string& name, int hp, int attack)
    : name(name), hp(hp), attack(attack) {}

void Character::equip(std::unique_ptr<Equipment> item) {
    if (equippedItem) {
        unequip();
    }
    equippedItem = std::move(item);
    attack += equippedItem->getAttackBonus();
    hp += equippedItem->getHpBonus();
}

void Character::unequip() {
    if (equippedItem) {
        attack -= equippedItem->getAttackBonus();
        hp -= equippedItem->getHpBonus();
        equippedItem.reset();
    }
}

void Character::takeDamage(int dmg) {
    hp -= dmg;
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

std::string Character::getName() const {
    return name;
}

void Character::increaseAttack(int amount) {
    attack += amount;
}
