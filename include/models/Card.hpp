#pragma once

#include <bits/stdc++.h>
// #include "models/Player.hpp"
// #include "core/GameManager.hpp"

class Player;
class GameManager;

class Card {
protected:
    std::string description;
public:
    Card(std::string desc);
    virtual ~Card() {}
    virtual void execute(Player& player, GameManager& gm) = 0;
    std::string getDescription() const;
};