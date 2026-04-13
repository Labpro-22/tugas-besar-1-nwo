#pragma once

#include "models/Tile.hpp"


class FestivalTile : public Tile {
public:
    FestivalTile(int idx, std::string c, std::string n);
    void onLanded(Player& player, GameManager& gm) override;
};