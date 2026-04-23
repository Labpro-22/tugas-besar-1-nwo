#include "models/GoToJailTile.hpp"

GoToJailTile::GoToJailTile(int idx, std::string c, std::string n):Tile(idx,c,n,"GOTOJAIL"){}
// --- GO TO JAIL TILE (Kena Razia) ---
void GoToJailTile::onLanded(Player& player, GameManager& gm) {
    gm.getLogger().logAction(gm.getCurrentTurnCount(), player.getUsername(), "ARRESTED", "Kena razia! Masuk penjara!");
    int jailIdx = gm.getBoard().getTileIndexByCode("JAIL"); 
    player.setPosition(jailIdx != -1 ? jailIdx : 10); 
    player.setStatus("JAILED"); // Status ini akan memicu menu penjara di giliran berikutnya
}