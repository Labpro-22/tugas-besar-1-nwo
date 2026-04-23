#include "models/FreeParkingTile.hpp"
#include "models/Player.hpp"

using namespace std;
FreeParkingTile::FreeParkingTile(int idx, string c, string n) 
    : Tile(idx, c, n, "FREEPARKING") {}
// --- FREE PARKING ---
void FreeParkingTile::onLanded(Player& player, GameManager& gm) {
    gm.getLogger().logAction(gm.getCurrentTurnCount(), player.getUsername(), "REST", "Istirahat sejenak di Parkir Gratis.");
    player.setStatus("TURN_ENDED");
}