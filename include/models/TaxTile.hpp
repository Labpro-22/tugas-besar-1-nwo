#pragma once

#include "models/Tile.hpp"
#include "models/PropertyTile.hpp"
#include "utils/utils.hpp"

class TaxTile : public Tile {
private:
    std::string taxType;
    int flatTax;
    int percentageTax;
public:
    int getFlatTax() const { return flatTax; }
    int getPercentageTax() const { return percentageTax; }
    TaxTile(int idx, std::string c, std::string n, std::string type, int flat, int pct);
    void onLanded(Player& player, GameManager& gm) override;
    TaxTile* getAsTax() override { return this; }
};