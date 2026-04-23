#include "models/StreetTile.hpp"
#include "utils/Dice.hpp"

StreetTile::StreetTile(int idx, std::string c, std::string n, int price, int mortgage, std::string color, int hPrice, std::vector<int> rents)
    : PropertyTile(idx, c, n, "STREET", price, mortgage), colorGroup(color), 
      rentTable(rents), housePrice(hPrice), buildingCount(0), 
      festivalMultiplier(1), festivalDuration(0) {}

int StreetTile::calculateRent(Dice& dice, int ownerPropertyCount) override {
    int baseRent = rentTable[buildingCount];
    // Jika monopoli warna (asumsi ownerPropertyCount dicek di manager), baseRent * 2
    return baseRent * festivalMultiplier;
}