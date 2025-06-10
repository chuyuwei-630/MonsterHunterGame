#include "Character.h"
#include <iostream>
#include <random>

Character::Character(const std::string& name, int hp, int attack, int defense)
    : name(name), hp(hp), attack(attack), defense(defense), hasRevived(false),
      baseHp(hp), baseAttack(attack), baseDefense(defense), hasRepairStone(false) {}

void Character::equip(std::unique_ptr<Equipment> item) {
    Equipment::Type type = item->getType();
    if (equippedItems.find(type) != equippedItems.end()) {
        unequip(type);
    }
    equippedItems[type] = std::move(item);
    attack += equippedItems[type]->getAttackBonus();
    hp += equippedItems[type]->getHpBonus();
    defense += equippedItems[type]->getDefenseBonus();
    if (equippedItems[type]->hasLegendaryEffect() && type == Equipment::ACCESSORY) {
        attack += 10;
        hp += 10;
        defense += 10;
    }
    if (equippedItems[type]->hasLegendaryEffect() && type == Equipment::GLOVES) {
        attack += 5;
    }
}

void Character::unequip(Equipment::Type type) {
    if (equippedItems.find(type) != equippedItems.end()) {
        attack -= equippedItems[type]->getAttackBonus();
        hp -= equippedItems[type]->getHpBonus();
        defense -= equippedItems[type]->getDefenseBonus();
        if (equippedItems[type]->hasLegendaryEffect() && type == Equipment::ACCESSORY) {
            attack -= 10;
            hp -= 10;
            defense -= 10;
        }
        if (equippedItems[type]->hasLegendaryEffect() && type == Equipment::GLOVES) {
            attack -= 5;
        }
        addToInventory(std::move(equippedItems[type]));
        equippedItems.erase(type);
    }
}

void Character::addToInventory(std::unique_ptr<Equipment> item) {
    inventory.push_back(std::move(item));
}

void Character::equipFromInventory(size_t index) {
    if (index < inventory.size()) {
        equip(std::move(inventory[index]));
        inventory.erase(inventory.begin() + index);
    } else {
        std::cout << "無效的庫存索引！" << std::endl;
    }
}

int Character::takeDamage(int dmg, Character* attacker) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dodge_dist(1, 100);
    if (hasLegendaryShield() && dodge_dist(gen) <= 25) {
        std::cout << "傳說盾牌發動閃避效果，你完全躲避了這次攻擊！" << std::endl;
        return 0;
    }

    int actualDamage = dmg - defense;
    if (actualDamage < 0) actualDamage = 0;
    hp -= actualDamage;
    if (hp < 0) hp = 0;

    if (hasLegendaryArmor() && attacker != nullptr) {
        int reflectDamage = static_cast<int>(actualDamage * 0.25);
        if (reflectDamage > 0) {
            attacker->takeDamage(reflectDamage);
            std::cout << "傳說護甲反彈 " << reflectDamage << " 點傷害給 " << attacker->getName() << "！" << std::endl;
        }
    }

    if (hp == 0 && hasLegendaryWeapon() && !hasRevived) {
        revive();
    }

    checkEquipmentBreak();
    return actualDamage;
}

bool Character::checkEquipmentBreak() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> break_dist(1, 100);
    bool anyBroken = false;

    for (auto it = equippedItems.begin(); it != equippedItems.end();) {
        Equipment* item = it->second.get();
        int breakChance = 0;
        switch (item->getRarity()) {
            case Equipment::COMMON: breakChance = 25; break;
            case Equipment::RARE: breakChance = 20; break;
            case Equipment::EPIC: breakChance = 15; break;
            case Equipment::LEGENDARY: breakChance = 10; break;
        }
        if (!item->isBroken() && break_dist(gen) <= breakChance) {
            item->breakItem();
            std::cout << "[" << item->getRarityString() << "]" << item->getName() << " 已損壞，失去效果！" << std::endl;
            unequip(it->first);
            it = equippedItems.begin();
            anyBroken = true;
        } else {
            ++it;
        }
    }
    return anyBroken;
}

bool Character::isAlive() const { return hp > 0; }
int Character::getAttack() const { return attack; }
int Character::getHP() const { return hp; }
int Character::getDefense() const { return defense; }
std::string Character::getName() const { return name; }
void Character::increaseAttack(int amount) { attack += amount; baseAttack += amount; }

void Character::displayInventory() const {
    std::cout << "庫存中的裝備：" << std::endl;
    if (inventory.empty()) {
        std::cout << "（空）" << std::endl;
    } else {
        for (size_t i = 0; i < inventory.size(); ++i) {
            std::cout << i << ": [" << inventory[i]->getRarityString() << "] " << inventory[i]->getName()
                      << " (攻擊力+" << inventory[i]->getAttackBonus()
                      << ", HP+" << inventory[i]->getHpBonus()
                      << ", 防禦+" << inventory[i]->getDefenseBonus() << ")" << std::endl;
        }
    }
}

bool Character::hasLegendaryWeapon() const {
    auto it = equippedItems.find(Equipment::WEAPON);
    return it != equippedItems.end() && it->second->hasLegendaryEffect();
}

bool Character::hasLegendaryArmor() const {
    auto it = equippedItems.find(Equipment::ARMOR);
    return it != equippedItems.end() && it->second->hasLegendaryEffect();
}

bool Character::hasLegendaryShield() const {
    auto it = equippedItems.find(Equipment::SHIELD);
    return it != equippedItems.end() && it->second->hasLegendaryEffect();
}

bool Character::hasLegendaryRing() const {
    auto it = equippedItems.find(Equipment::ACCESSORY);
    return it != equippedItems.end() && it->second->hasLegendaryEffect();
}

bool Character::hasLegendaryGloves() const {
    auto it = equippedItems.find(Equipment::GLOVES);
    return it != equippedItems.end() && it->second->hasLegendaryEffect();
}

void Character::revive() {
    hp = 1;
    hasRevived = true;
    std::cout << "傳說武器發動復活效果，你的生命值恢復至 1！" << std::endl;
}

void Character::applyEvolutionBoost() {
    hp = static_cast<int>(hp * 1.5);
    attack = static_cast<int>(attack * 1.5);
    defense = static_cast<int>(defense * 1.5);
}

void Character::resetEvolutionBoost() {
    hp = baseHp;
    attack = baseAttack;
    defense = baseDefense;
    for (const auto& [type, item] : equippedItems) {
        attack += item->getAttackBonus();
        hp += item->getHpBonus();
        defense += item->getDefenseBonus();
        if (item->hasLegendaryEffect() && type == Equipment::ACCESSORY) {
            attack += 10;
            hp += 10;
            defense += 10;
        }
        if (item->hasLegendaryEffect() && type == Equipment::GLOVES) {
            attack += 5;
        }
    }
}

void Character::setRepairStone(bool has) {
    hasRepairStone = has;
}



void Character::repairWeapon() {
    auto it = equippedItems.find(Equipment::WEAPON);
    if (it != equippedItems.end() && it->second->isBroken()) {
        it->second->repairItem();
        attack += it->second->getAttackBonus();
        hp += it->second->getHpBonus();
        defense += it->second->getDefenseBonus();
        hasRepairStone = false;
        std::cout << "武器已修復！" << std::endl;
    } else {
        std::cout << "沒有損壞的武器可修復！" << std::endl;
    }
}

Player::Player(const std::string& name, int hp, int attack, int defense)
    : Character(name, hp, attack, defense) {}

void Player::heal(int amount) {
    hp += amount;
    if (hp > 100) hp = 100;
    baseHp = hp;
}

Element Player::getEquippedWeaponElement() const {
    auto it = equippedItems.find(Equipment::WEAPON);
    if (it != equippedItems.end()) {
        return it->second->getElement();
    }
    return Element::NONE;
}