#include "models/RailroadTile.hpp"
#include "utils/Dice.hpp"

RailroadTile::RailroadTile(int idx, std::string c, std::string n, int price, int mortgage, std::map<int, int> rents)
    : PropertyTile(idx, c, n, "RAILROAD", price, mortgage), rentTable(rents) {}

int RailroadTile::calculateRent(Dice& dice, int ownerPropertyCount) override {
    return rentTable[ownerPropertyCount];
}