#include "models/GoTile.hpp"
#include "models/Player.hpp"
using namespace std;
GoTile::GoTile(int idx, string c, string n, int salary) 
    : Tile(idx, c, n, "GO"), salaryAmount(salary) {}
// --- GO TILE ---
void GoTile::onLanded(Player& player, GameManager& gm) {
    gm.getLogger().logAction(gm.getCurrentTurnCount(), player.getUsername(), "GO", "Mendarat tepat di START!");
    player.setStatus("TURN_ENDED");
}
void GoTile::passGo(Player& player, GameManager& gm) {
    player += salaryAmount;
    gm.getLogger().logAction(gm.getCurrentTurnCount(), player.getUsername(), "CASH", "Lewat START! Gaji M" + to_string(salaryAmount) + " cair.");
}