#pragma once

#include <bits/stdc++.h>

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
};