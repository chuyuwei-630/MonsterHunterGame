#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <string>

class Equipment {
public:
    enum Type { WEAPON, ARMOR };

    Equipment(const std::string& name, Type type, int attackBonus, int hpBonus);

    std::string getName() const;
    Type getType() const;
    int getAttackBonus() const;
    int getHpBonus() const;

private:
    std::string name;
    Type type;
    int attackBonus;
    int hpBonus;
};

#endif // EQUIPMENT_H

