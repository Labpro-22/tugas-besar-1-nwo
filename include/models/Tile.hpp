#pragma once

#include <bits/stdc++.h>
#include <raylib.h>
#include "utils/Dice.hpp"
// #include "core/GameManager.hpp"
// #include "models/Player.hpp"
class GameManager;
class Player;


class Tile {
protected:
    int index;
    std::string code;
    std::string name;
    std::string type;
public:
    Tile(int idx, std::string c, std::string n, std::string t);
    virtual ~Tile() {}
    
    int getIndex() const;
    std::string getCode() const;
    std::string getName() const;
    std::string getType() const;
    virtual void onLanded(Player& player, GameManager& gm) = 0;
    virtual bool hasHeaderColor() const { return false; }
    virtual Color getHeaderColor() const { return BLANK; }
    virtual std::string getExtraDetails() const { return ""; }
    virtual bool isProperty() const { return false; }
    virtual int getBuyPrice() const { return 0; }
    virtual std::string getOwner() const { return "SYSTEM"; }
    virtual int getBuildingCount() const { return 0; }
    virtual int getUpgradePrice() const { return 0; } // FAKTA: Harga bangun rumah
    virtual bool isStreet() const { return false; }
    virtual bool canBuild() const { return false; }
    virtual void build(){};
    virtual bool canUpgrade(const GameManager&) const { return false; }
    virtual bool isFestivalActive() const { return false; }
    virtual int getCurrentRent(const Dice& , const GameManager&) const { return 0; }
    
};