#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <string>
#include <memory>

class Equipment {
public:
    enum Type { WEAPON, ARMOR, ACCESSORY, HELMET, BOOTS, GLOVES };
    enum Rarity { COMMON, RARE, EPIC, LEGENDARY };

    Equipment(const std::string& name, Type type, Rarity rarity, int attackBonus, int hpBonus, int defenseBonus, int hpRegen);
    std::string getName() const;
    Type getType() const;
    Rarity getRarity() const;
    int getAttackBonus() const;
    int getHpBonus() const;
    int getDefenseBonus() const;
    int getHpRegen() const;

private:
    std::string name;
    Type type;
    Rarity rarity;
    int attackBonus;
    int hpBonus;
    int defenseBonus;
    int hpRegen;
};

#endif

