#include <iostream>
#include <string>
using namespace std;

// 定義角色類別（可以用於玩家和怪物）
class Character {
public:
    string name;
    int health;
    int attack;

    Character(string n, int h, int a) : name(n), health(h), attack(a) {}

    // 攻擊對方
    void attackEnemy(Character& enemy) {
        enemy.health -= attack;
        cout << name << " attacks " << enemy.name << " for " << attack << " damage!" << endl;
        if (enemy.health < 0) enemy.health = 0;
        cout << enemy.name << " now has " << enemy.health << " health." << endl;
    }

    // 檢查是否存活
    bool isAlive() {
        return health > 0;
    }
};

int main() {
    // 創建玩家和怪物
    Character player("Hero", 100, 20);
    Character monster("Dragon", 80, 15);

    cout << "A battle begins between " << player.name << " and " << monster.name << "!" << endl;

    // 戰鬥迴圈
    while (player.isAlive() && monster.isAlive()) {
        // 玩家攻擊怪物
        player.attackEnemy(monster);
        if (!monster.isAlive()) {
            cout << monster.name << " has been defeated! " << player.name << " wins!" << endl;
            break;
        }

        // 怪物反擊
        monster.attackEnemy(player);
        if (!player.isAlive()) {
            cout << player.name << " has been defeated! " << monster.name << " wins!" << endl;
            break;
        }

        cout << "-------------------" << endl;
    }

    return 0;
}