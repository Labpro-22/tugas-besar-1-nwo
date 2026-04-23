#pragma once

#include "models/Tile.hpp"
#include "models/PropertyTile.hpp"
// #include "utils/utils.hpp"
// #include "utils/Dice.hpp"
class RailroadTile : public PropertyTile {
private:
    std::map<int, int> rentTable; 
public:
    RailroadTile(int idx, std::string c, std::string n, int price, int mortgage, std::map<int, int> rents);
    int calculateRent(const Dice& dice, int ownerPropertyCount) const override;
    int getCurrentRent(const Dice& d, const GameManager& gm) const override;
    void onLanded(Player& player, GameManager& gm) override;
    bool hasHeaderColor() const override { return true; }
    Color getHeaderColor() const override { return DARKGRAY; }
    void build() override{};
};
