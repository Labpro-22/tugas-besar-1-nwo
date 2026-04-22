#pragma once

#include "models/Tile.hpp"
// #include "utils/Dice.hpp"

// class Dice;
class Player;
class GameManager;
class Dice;
// class Tile;

class PropertyTile : public Tile {
protected:
    int buyPrice;
    int mortgageValue;
    std::string ownerName;
    std::string propertyStatus; 
public:
    PropertyTile(int idx, std::string c, std::string n, std::string t, int price, int mortgage);
    
    virtual int calculateRent(Dice& dice, int ownerPropertyCount) = 0;
    
    int getBuyPrice() const;
    int getMortgageValue() const;
    std::string getOwner() const;
    void setOwner(std::string owner);
    std::string getPropertyStatus() const;
    
    void mortgageProperty();
    void redeemProperty();
    void onLanded(Player& player, GameManager& gm) override;
};