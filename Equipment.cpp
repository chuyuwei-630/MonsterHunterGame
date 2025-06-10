#include "Equipment.h"

Equipment::Equipment(const std::string& name, Type type, int attackBonus, int hpBonus, int defenseBonus, Rarity rarity, Element element)
    : name(name), type(type), attackBonus(attackBonus), hpBonus(hpBonus), defenseBonus(defenseBonus), rarity(rarity), element(element) {}

std::string Equipment::getName() const { return name; }
Equipment::Type Equipment::getType() const { return type; }
int Equipment::getAttackBonus() const { return attackBonus; }
int Equipment::getHpBonus() const { return hpBonus; }
int Equipment::getDefenseBonus() const { return defenseBonus; }
Equipment::Rarity Equipment::getRarity() const { return rarity; }
Element Equipment::getElement() const { return element; }

std::string Equipment::getRarityString() const {
    switch (rarity) {
        case COMMON: return "普通";
        case RARE: return "稀有";
        case EPIC: return "史詩";
        case LEGENDARY: return "傳說";
        default: return "未知";
    }
}