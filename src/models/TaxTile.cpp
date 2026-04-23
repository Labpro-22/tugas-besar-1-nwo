#include "models/TaxTile.hpp"
#include "utils/Dice.hpp"

TaxTile::TaxTile(int idx, std::string c, std::string n, std::string type, int flat, int pct)
    : Tile(idx, c, n, "TAX"), taxType(type), flatTax(flat), percentageTax(pct) {}

void TaxTile::onLanded(Player& player, GameManager& gm) {
    if (taxType == "PPH") {
        // Tawarkan pilihan: Bayar flat atau % dari total kekayaan
    } else {
        // PBM: Langsung potong flatTax
    }
}