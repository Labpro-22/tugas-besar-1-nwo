#include "models/CardTile.hpp"

CardTile::CardTile(int idx, std::string c, std::string n, std::string type) 
    : Tile(idx, c, n, type), deckType(type) {}

void CardTile::onLanded(Player& player, GameManager& gm) {
    // Logika mengambil kartu dari deck sesuai deckType (CHANCE/COMMUNITY_CHEST)
    // Lalu memanggil card->execute(player, gm)
}