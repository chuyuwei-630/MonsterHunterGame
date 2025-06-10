#include "Game.h"
#include "GameWithMap.h"
#include "Equipment.h"
#include <iostream>
#include <windows.h>

int main() {
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "選擇遊戲模式：1 - 戰鬥遊戲, 2 - 地圖遊戲: ";
    int choice;
    std::cin >> choice;

    if (choice == 1) {
        Game game;
        game.start();
    } else if (choice == 2) {
        Player player("PlayerName", 100, 25, 0);
        GameWithMap game(player);
        game.start();
    } else {
        std::cout << "無效選擇！" << std::endl;
    }

    return 0;
}
