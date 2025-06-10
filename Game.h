#ifndef GAME_H
#define GAME_H

#include "Character.h"
#include <memory>
#include <string>
#include <chrono>

class Game {
private:
    std::unique_ptr<Player> player;
    std::unique_ptr<Character> monster;
    int score;
    std::chrono::time_point<std::chrono::steady_clock> startTime;

public:
    Game();
    ~Game();

    void start();
    void battle();

private:
    void generateMonster();
};

void slowPrint(const std::string& text, int delay = 50);

#endif

