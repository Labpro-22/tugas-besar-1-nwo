#pragma once

#include "models/Tile.hpp"
#include "core/GameManager.hpp"

class GoTile : public Tile {
private:
    int salaryAmount;
public:
    GoTile(int idx, std::string c, std::string n, int salary);
    void onLanded(Player& player, GameManager& gm) override;
    void passGo(Player& player, GameManager& gm);
    void build() override{};
};