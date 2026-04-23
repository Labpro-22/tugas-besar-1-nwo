#pragma once

#include "models/Tile.hpp"
#include "models/PropertyTile.hpp"
#include "utils/utils.hpp"

class UtilityTile : public PropertyTile {
private:
    std::map<int, int> multiplierTable;
public:
    UtilityTile(int idx, std::string c, std::string n, int price, int mortgage, std::map<int, int> multipliers);
    
    int calculateRent(Dice& dice, int ownerPropertyCount) override;
    void onLanded(Player& player, GameManager& gm) override;
};