#include "Equipment.h"

Equipment::Equipment(const std::string& name, Type type, int attackBonus, int hpBonus, int defenseBonus, int hpRegen)
    : name(name), type(type), attackBonus(attackBonus), hpBonus(hpBonus), defenseBonus(defenseBonus), hpRegen(hpRegen) {}

std::string Equipment::getName() const {
    return name;
}

Equipment::Type Equipment::getType() const {
    return type;
}

int Equipment::getAttackBonus() const {
    return attackBonus;
}

int Equipment::getHpBonus() const {
    return hpBonus;
}

int Equipment::getDefenseBonus() const {
    return defenseBonus;
}

int Equipment::getHpRegen() const {
    return hpRegen;
}
