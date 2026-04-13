#pragma once

#include <bits/stdc++.h>

class GameCLI {
private:
    GameManager& gm;
public:
    GameCLI(GameManager& manager);
    void runMenu();
    void displayBoard();
    void displayPropertyDeed(std::string code);
    void displayPlayerProperties(const Player& player);
    void promptLiquidation(Player& player, int deficit);
};