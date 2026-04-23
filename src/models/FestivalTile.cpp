#include "models/FestivalTile.hpp"
#include "models/Player.hpp"
#include "core/GameManager.hpp"

FestivalTile::FestivalTile(int idx, std::string c, std::string n) 
    : Tile(idx, c, n, "FESTIVAL") {}

// --- LOGIKA ON LANDED (Versi Raylib) ---
void FestivalTile::onLanded(Player& player, GameManager& gm) {
    bool hasValidCity = false;

    // FAKTA: Kita filter! Cari apakah dia punya minimal 1 petak jalanan (StreetTile)
    for (const PropertyTile* prop : player.getOwnedProperties()) {
        if (prop->isStreet()) { // Gak bakal ketipu sama PLN atau Stasiun lagi
            hasValidCity = true;
            break;
        }
    }

    if (hasValidCity) {
        // Kalau punya kota, lempar ke UI Pemilihan Festival
        player.setStatus("FESTIVAL_SELECT");
        gm.getLogger().logAction(gm.getCurrentTurnCount(), player.getUsername(), "FESTIVAL", "Pilih kota untuk diadakan Festival!");
    } else {
        // Kalau cuma punya PLN/Stasiun, atau murni gelandangan gak punya aset
        player.setStatus("TURN_ENDED");
        gm.getLogger().logAction(gm.getCurrentTurnCount(), player.getUsername(), "FESTIVAL", "Lewat: Tidak ada kota untuk festival.");
    }
}