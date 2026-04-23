#pragma once

#include <bits/stdc++.h>
// #include "models/Tile.hpp"
#include "models/PropertyTile.hpp"
#include <raylib.h>
// #include "utils/utils.hpp"
// #include "utils/Dice.hpp"
// #include "core/GameManager.hpp"
class Dice;
class Player;
class GameManager;

class UtilityTile : public PropertyTile {
private:
    std::map<int, int> multiplierTable;
public:
    UtilityTile(int idx, std::string c, std::string n, int price, int mortgage, std::map<int, int> multipliers);
    int calculateRent(const Dice& dice, int ownerPropertyCount) const override;
    void onLanded(Player& player, GameManager& gm) override;
    bool hasHeaderColor() const override { return true; }
    Color getHeaderColor() const override { return GRAY; }
    int getCurrentRent(const Dice& d, const GameManager& gm) const override;
    void build() override{};
};