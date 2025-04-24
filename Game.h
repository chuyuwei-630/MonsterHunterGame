#ifndef GAME_H
#define GAME_H

#include "Character.h"

class Game {
private:
    Character* player;
    Character* monster;

public:
    Game();
    ~Game();

    void start();
    void battle();
};

#endif
