#ifndef MONSTER_H
#define MONSTER_H

class Monster {

private:
    int health;
    int attack;

public:
    Monster(int h, int a) : health(h), attack(a) {}


    void attackEnemy(Character& enemy) {
        enemy.health -= attack;
        if (enemy.health < 0) enemy.health = 0;
    }
};
};
bool isAlive() {
    return health > 0;
}
int getAttack() {
    return attack;
}
int getHealth() {
    return health;
}
void setHealth(int h) {
    health = h;
}
void setAttack(int a) {
    attack = a;
}
void displayStats() {
    cout << "Monster Health: " << health << endl;
    cout << "Monster Attack: " << attack << endl;
}
