#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>

class Character {
protected:
    std::string name;
    int hp;
    int attack;

public:
    Character(const std::string& name, int hp, int attack);
    virtual ~Character() = default;

    void takeDamage(int dmg);
    bool isAlive() const;

    int getAttack() const;
    std::string getName() const;
    int getHP() const;
};

#endif

