#include "models/FestivalTile.hpp"
#include "models/Player.hpp"
#include "core/GameManager.hpp"

FestivalTile::FestivalTile(int idx, std::string c, std::string n) 
    : Tile(idx, c, n, "FESTIVAL") {}

void FestivalTile::onLanded(Player& player, GameManager& gm) {
    bool hasValidCity = false;

    
    for (const PropertyTile* prop : player.getOwnedProperties()) {
        if (prop->isStreet()) { 
            hasValidCity = true;
            break;
        }
    }

    if (hasValidCity) {
        
        player.setStatus("FESTIVAL_SELECT");
        gm.getLogger().logAction(gm.getCurrentTurnCount(), player.getUsername(), "FESTIVAL", "Pilih kota untuk diadakan Festival!");
    } else {
       
        player.setStatus("TURN_ENDED");
        gm.getLogger().logAction(gm.getCurrentTurnCount(), player.getUsername(), "FESTIVAL", "Lewat: Tidak ada kota untuk festival.");
    }
}