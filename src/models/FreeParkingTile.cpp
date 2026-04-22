#include "models/FreeParkingTile.hpp"

FreeParkingTile::FreeParkingTile(int idx, std::string c, std::string n) 
    : Tile(idx, c, n, "FREE_PARKING") {}

void FreeParkingTile::onLanded(Player& player, GameManager& gm) {
    // Tidak melakukan apa-apa (Rest Area)
}