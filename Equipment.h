#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <string>

class Equipment {
public:
    enum Type { WEAPON, ARMOR, ACCESSORY };

    Equipment(const std::string& name, Type type, int attackBonus, int hpBonus, int defenseBonus, int hpRegen);

    std::string getName() const;
    Type getType() const;
    int getAttackBonus() const;
    int getHpBonus() const;
    int getDefenseBonus() const;
    int getHpRegen() const;

private:
    std::string name;
    Type type;
    int attackBonus;
    int hpBonus;
    int defenseBonus;
    int hpRegen;
};

#endif // EQUIPMENT_H
