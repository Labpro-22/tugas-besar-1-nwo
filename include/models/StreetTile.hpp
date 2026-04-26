#pragma once

#include "models/Tile.hpp"
#include "utils/utils.hpp"
#include "models/PropertyTile.hpp"
class StreetTile : public PropertyTile {
private:
    std::string colorGroup;
    std::vector<int> rentTable; 
    int housePrice;
    int hotelPrice;
    int buildingCount; 
    int festivalMultiplier;
    int festivalDuration;
    
public:

    StreetTile(int idx, std::string c, std::string n, int price, int mortgage, std::string color, int hPrice, int htPrice, std::vector<int> rents);
    void buildHouse();
    void demolishBuilding();
    void onLanded(Player& player, GameManager& gm) override;
    void applyFestival(int multiplier, int duration);
    void decrementFestival();
    std::string getColorGroup() const;
    int getTotalBuildingValue() const;
    bool hasHeaderColor() const override { return true; }
    std::string getExtraDetails() const override {
        return "Level Bangunan: L" + std::to_string(buildingCount);
    }
    Color getHeaderColor() const override;
    int getBuildingCount() const override;
    int getHouseCount() const override { return buildingCount; }
    int getHousePrice() const override { return housePrice; }
    int getHotelPrice() const override { return hotelPrice; }
    int getUpgradePrice() const override { 
        return (buildingCount == 4) ? hotelPrice : housePrice; 
    }
    bool isStreet() const override { return true; }
    bool canBuild() const override { return buildingCount < 5; } 
    void build() override { buildHouse(); }
    int calculateRent(const Dice& , int) const override {
        if (propertyStatus == "MORTGAGED") return 0;
        return rentTable.at(buildingCount) * festivalMultiplier;
    }
    int getCurrentRent(const Dice& d, const GameManager&) const override {
        return this->calculateRent(d, 0); 
    }
    bool canUpgrade(const GameManager& gm) const override;
};
