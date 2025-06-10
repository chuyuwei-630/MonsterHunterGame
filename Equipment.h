#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <string>

enum class Element { NONE, FIRE, WATER, EARTH, AIR };

class Equipment {
public:
    enum Type { WEAPON, ARMOR, ACCESSORY, SHIELD, GLOVES };
    enum Rarity { COMMON, RARE, EPIC, LEGENDARY };

    Equipment(const std::string& name, Type type, int attackBonus, int hpBonus, int defenseBonus, Rarity rarity, Element element = Element::NONE);
    std::string getName() const;
    Type getType() const;
    int getAttackBonus() const;
    int getHpBonus() const;
    int getDefenseBonus() const;
    Rarity getRarity() const;
    Element getElement() const;
    std::string getRarityString() const;
    bool isBroken() const;
    void breakItem();
    bool hasLegendaryEffect() const;

private:
    std::string name;
    Type type;
    int attackBonus;
    int hpBonus;
    int defenseBonus;
    Rarity rarity;
    Element element;
    bool broken;
};

#endif // EQUIPMENT_H