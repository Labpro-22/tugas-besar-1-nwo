#include "models/FestivalTile.hpp"

FestivalTile::FestivalTile(int idx, std::string c, std::string n) 
    : Tile(idx, c, n, "FESTIVAL") {}

void FestivalTile::onLanded(Player& player, GameManager& gm) {
    // UI memanggil daftar properti pemain, player memilih, 
    // lalu panggil properti->applyFestival()
}