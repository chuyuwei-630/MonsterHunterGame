#ifndef GAME_H
#define GAME_H

#include "Character.h"
#include <memory>

class Game {
private:
    std::unique_ptr<Character> player;
    std::unique_ptr<Character> monster;

public:
    Game();
    ~Game();

    void start();
    void battle();
};

#endif
