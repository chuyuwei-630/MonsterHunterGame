#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <string>

class Equipment {
public:
    enum Type { WEAPON, ARMOR, ACCESSORY, SHIELD }; // Added SHIELD type

    Equipment(const std::string& name, Type type, int attackBonus, int hpBonus, int defenseBonus = 0);

    std::string getName() const;
    Type getType() const;
    int getAttackBonus() const;
    int getHpBonus() const;
    int getDefenseBonus() const;

private:
    std::string name;
    Type type;
    int attackBonus;
    int hpBonus;
    int defenseBonus;
};

#endif // EQUIPMENT_H
