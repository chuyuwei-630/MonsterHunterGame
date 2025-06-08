#include "Character.h"

Character::Character(const std::string& name, int hp, int attack)
    : name(name), hp(hp), attack(attack) {}

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

std::string Character::getName() const {
    return name;
}

int Character::getHP() const {
    return hp;
}

void Character::increaseAttack(int amount) {
    attack += amount;
}