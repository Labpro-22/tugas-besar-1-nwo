#pragma once

#include "models/Tile.hpp"

class GoToJailTile : public Tile {
public:
    GoToJailTile(int idx, std::string c, std::string n);
    void onLanded(Player& player, GameManager& gm) override;
};
