#pragma once

#include "models/Tile.hpp"

class JailTile : public Tile {
private:
    int bailAmount;
public:
    JailTile(int idx, std::string c, std::string n, int bail);
    void onLanded(Player& player, GameManager& gm) override;
    void processInmate(Player& player, GameManager& gm);
};
