#pragma once

#include "models/Tile.hpp"
#include "utils/utils.hpp"
class StreetTile : public PropertyTile {
private:
    std::string colorGroup;
    std::vector<int> rentTable; 
    int housePrice;
    int buildingCount; 
    int festivalMultiplier;
    int festivalDuration;

public:
    StreetTile(int idx, std::string c, std::string n, int price, int mortgage, std::string color, int hPrice, std::vector<int> rents);
    
    int calculateRent(Dice& dice, int ownerPropertyCount) override;
    void buildHouse();
    void demolishBuilding();
    
    void applyFestival(int multiplier, int duration);
    void decrementFestival();
    
    std::string getColorGroup() const;
    int getBuildingCount() const;
    int getTotalBuildingValue() const;
};
