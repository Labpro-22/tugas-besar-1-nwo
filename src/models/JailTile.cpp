#include "models/JailTile.hpp"
#include "core/GameManager.hpp"
#include <iostream>
using namespace std;
JailTile::JailTile(int idx, string c, string n, int bail) 
    : Tile(idx, c, n, "JAIL"), bailAmount(bail) {}

void JailTile::onLanded(Player& player, GameManager& gm) {
    if (player.getStatus() == "JAILED") {
        gm.getLogger().logAction(gm.getCurrentTurnCount(), player.getUsername(), "JAIL", "Masih mendekam di penjara.");
    } else {
        gm.getLogger().logAction(gm.getCurrentTurnCount(), player.getUsername(), "INFO", "Hanya berkunjung di penjara.");
        player.setStatus("TURN_ENDED");
    }
}
void JailTile::processInmate(Player& player, GameManager& gm) {
    
    player.setStatus("JAIL_CHOICE");
}