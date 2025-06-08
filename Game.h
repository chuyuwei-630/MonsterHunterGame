<<<<<<< HEAD
#ifndef GAME_H
#define GAME_H

#include "Character.h"
#include <memory>
#include <string>

class Game {
private:
    std::unique_ptr<Character> player;
    std::unique_ptr<Character> monster;
    int score;

public:
    Game();
    ~Game();

    void start();
    void battle();

private:
    void generateMonster();  // 🔧 ← 加這一行來修正編譯錯誤
};

// ✅ 正確位置：在 Game 類別「外面」宣告 slowPrint
void slowPrint(const std::string& text, int delay = 50);

#endif


=======
#ifndef GAME_H
#define GAME_H

#include "Character.h"
#include <memory>
#include <string>

class Game {
private:
    std::unique_ptr<Character> player;
    std::unique_ptr<Character> monster;
    int score;

public:
    Game();
    ~Game();

    void start();
    void battle();

private:
    void generateMonster();  // 🔧 ← 加這一行來修正編譯錯誤
};

// ✅ 正確位置：在 Game 類別「外面」宣告 slowPrint
void slowPrint(const std::string& text, int delay = 50);

#endif


>>>>>>> 1375e4f2c47132a367752508f7f518dbf4a6ab4b
