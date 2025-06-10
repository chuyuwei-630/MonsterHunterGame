#include "Equipment.h"

Equipment::Equipment(const std::string& name, Type type, Rarity rarity, int attackBonus, int hpBonus, int defenseBonus, int hpRegen)
    : name(name), type(type), rarity(rarity), attackBonus(attackBonus), hpBonus(hpBonus), defenseBonus(defenseBonus), hpRegen(hpRegen) {}

std::string Equipment::getName() const { return name; }
Equipment::Type Equipment::getType() const { return type; }
Equipment::Rarity Equipment::getRarity() const { return rarity; }

int Equipment::getAttackBonus() const {
    switch (rarity) {
        case COMMON: return attackBonus;
        case RARE: return attackBonus; // Values already adjusted in constructor
        case EPIC: return attackBonus;
        case LEGENDARY: return attackBonus;
        default: return attackBonus;
    }
}

int Equipment::getHpBonus() const {
    switch (rarity) {
        case COMMON: return hpBonus;
        case RARE: return hpBonus;
        case EPIC: return hpBonus;
        case LEGENDARY: return hpBonus;
        default: return hpBonus;
    }
}

int Equipment::getDefenseBonus() const {
    switch (rarity) {
        case COMMON: return defenseBonus;
        case RARE: return defenseBonus;
        case EPIC: return defenseBonus;
        case LEGENDARY: return defenseBonus;
        default: return defenseBonus;
    }
}

int Equipment::getHpRegen() const {
    switch (rarity) {
        case COMMON: return hpRegen;
        case RARE: return hpRegen;
        case EPIC: return hpRegen;
        case LEGENDARY: return hpRegen;
        default: return hpRegen;
    }
}
