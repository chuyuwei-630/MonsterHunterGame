<<<<<<< HEAD
#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>

class Character {
protected:
    std::string name;
    int hp;
    int attack;

public:
    // 建構子
    Character(const std::string& name, int hp, int attack);

    // 虛擬解構子（允許繼承類別正確清理）
    virtual ~Character() = default;

    // 傷害處理
    void takeDamage(int dmg);

    // 狀態查詢
    bool isAlive() const;
    int getAttack() const;
    int getHP() const;
    std::string getName() const;

    // 攻擊力增加
    void increaseAttack(int amount);
};

#endif // CHARACTER_H

=======
#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>

class Character {
protected:
    std::string name;
    int hp;
    int attack;

public:
    // 建構子
    Character(const std::string& name, int hp, int attack);

    // 虛擬解構子（允許繼承類別正確清理）
    virtual ~Character() = default;

    // 傷害處理
    void takeDamage(int dmg);

    // 狀態查詢
    bool isAlive() const;
    int getAttack() const;
    int getHP() const;
    std::string getName() const;

    // 攻擊力增加
    void increaseAttack(int amount);
};

#endif // CHARACTER_H

>>>>>>> 1375e4f2c47132a367752508f7f518dbf4a6ab4b
