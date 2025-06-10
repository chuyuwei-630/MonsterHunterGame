#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <map>
#include <vector>
#include <memory>
#include "Equipment.h"

class Character {
protected:
    std::string name;
    int hp;
    int attack;
    int defense;
    std::map<Equipment::Type, std::unique_ptr<Equipment>> equippedItems;
    std::vector<std::unique_ptr<Equipment>> inventory;
    bool hasRevived;
    int baseHp;
    int baseAttack;
    int baseDefense;
public:
    bool hasRepairStone; // 改為 public

    Character(const std::string& name, int hp, int attack, int defense = 0);
    virtual ~Character() = default;

    void equip(std::unique_ptr<Equipment> item);
    void unequip(Equipment::Type type);
    void addToInventory(std::unique_ptr<Equipment> item);
    void equipFromInventory(size_t index);
    int takeDamage(int dmg, Character* attacker = nullptr);
    bool isAlive() const;
    int getAttack() const;
    int getDefense() const;
    int getHP() const;
    std::string getName() const;
    void increaseAttack(int amount);
    void displayInventory() const;
    bool checkEquipmentBreak();
    bool hasLegendaryWeapon() const;
    bool hasLegendaryArmor() const;
    bool hasLegendaryShield() const;
    bool hasLegendaryRing() const;
    bool hasLegendaryGloves() const;
    void revive();
    void applyEvolutionBoost();
    void resetEvolutionBoost();
    void setRepairStone(bool has);
    void repairWeapon();
};

class Player : public Character {
public:
    Player(const std::string& name, int hp, int attack, int defense);
    void heal(int amount);
    Element getEquippedWeaponElement() const;
};

#endif