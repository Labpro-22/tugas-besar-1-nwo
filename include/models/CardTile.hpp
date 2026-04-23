#pragma once

#include "models/Tile.hpp"
// #include "core/GameManager.hpp"

class CardTile : public Tile {
private:
    std::string deckType; 
public:
    CardTile(int idx, std::string c, std::string n, std::string type);
    void onLanded(Player& player, GameManager& gm) override;
    void build() override{};
};