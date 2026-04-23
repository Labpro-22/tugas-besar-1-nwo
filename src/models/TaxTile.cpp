#include "models/TaxTile.hpp"
#include "utils/Dice.hpp"
#include "models/Player.hpp"
#include "core/GameManager.hpp"
using namespace std;
TaxTile::TaxTile(int idx, string c, string n, string type, int flat, int pct)
    : Tile(idx, c, n, "TAX"), taxType(type), flatTax(flat), percentageTax(pct) {}
void TaxTile::onLanded(Player& player, GameManager& gm) {
    // 1. Catat ke Log in-game
    gm.getLogger().logAction(gm.getCurrentTurnCount(), player.getUsername(), "INFO", "Mendarat di Petak Pajak!");

    // 2. FAKTA: Lempar status ke UI biar layarnya nampilin menu pilihan
    player.setStatus("TAX_CHOICE");
}