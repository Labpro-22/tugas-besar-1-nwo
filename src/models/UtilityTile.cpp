#include "models/UtilityTile.hpp"
#include "utils/Dice.hpp"

UtilityTile::UtilityTile(int idx, std::string c, std::string n, int price, int mortgage, std::map<int, int> multipliers)
    : PropertyTile(idx, c, n, "UTILITY", price, mortgage), multiplierTable(multipliers) {}

int UtilityTile::calculateRent(Dice& dice, int ownerPropertyCount) override {
    return dice.getTotal() * multiplierTable[ownerPropertyCount];
}