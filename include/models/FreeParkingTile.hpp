#pragma once

#include "models/Tile.hpp"
#include "core/GameManager.hpp"

class FreeParkingTile : public Tile {
public:
    FreeParkingTile(int idx, std::string c, std::string n);
    void onLanded(Player& player, GameManager& gm) override;
    void build() override{};
};