#pragma once

#include "models/Tile.hpp"
// #include "models/Player.hpp"
// #include "core/GameManager.hpp"

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
    virtual int calculateRent(const Dice& dice, int ownerPropertyCount) const = 0;
    int getMortgageValue() const;
    void setOwner(std::string owner);
    std::string getPropertyStatus() const;
    void mortgageProperty();
    void redeemProperty();
    void onLanded(Player& player, GameManager& gm) override;
    bool isProperty() const override { return true; }
    int getBuyPrice() const override;
    std::string getOwner() const override;
    PropertyTile* getAsProperty() override { return this; }
    virtual void applyFestival(int /*multiplier*/, int /*turns*/) {}
    virtual bool canBuild() const { return false; } 
    virtual int getHousePrice() const { return 0; }
    virtual int getHouseCount() const { return 0; }
    virtual void build() {}
    
};
