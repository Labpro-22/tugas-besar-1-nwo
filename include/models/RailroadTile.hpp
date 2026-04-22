#pragma once

#include "models/Tile.hpp"
#include "utils/utils.hpp"
class RailroadTile : public PropertyTile {
private:
    std::map<int, int> rentTable; 
public:
    RailroadTile(int idx, std::string c, std::string n, int price, int mortgage, std::map<int, int> rents);
    int calculateRent(Dice& dice, int ownerPropertyCount) override;
    void onLanded(Player& player, GameManager& gm) override;
};

class UtilityTile : public PropertyTile {
private:
    std::map<int, int> multiplierTable;
public:
    UtilityTile(int idx, std::string c, std::string n, int price, int mortgage, std::map<int, int> multipliers);
    int calculateRent(Dice& dice, int ownerPropertyCount) override;
    void onLanded(Player& player, GameManager& gm) override;
};

class TaxTile : public Tile {
private:
    std::string taxType;
    int flatTax;
    int percentageTax;
public:
    TaxTile(int idx, std::string c, std::string n, std::string type, int flat, int pct);
    void onLanded(Player& player, GameManager& gm) override;
};