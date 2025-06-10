#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <string>

class Equipment {
public:
    enum Type { WEAPON, ARMOR, ACCESSORY, SHIELD, GLOVES }; // Added GLOVES type
    enum Rarity { COMMON, RARE, EPIC, LEGENDARY }; // Added rarity tiers

    Equipment(const std::string& name, Type type, int attackBonus, int hpBonus, int defenseBonus, Rarity rarity);

    std::string getName() const;
    Type getType() const;
    int getAttackBonus() const;
    int getHpBonus() const;
    int getDefenseBonus() const;
    Rarity getRarity() const;
    std::string getRarityString() const;

private:
    std::string name;
    Type type;
    int attackBonus;
    int hpBonus;
    int defenseBonus;
    Rarity rarity;
};

#endif // EQUIPMENT_H
