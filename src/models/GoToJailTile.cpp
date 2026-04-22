#include "models/GoToJailTile.hpp"

GoToJailTile::GoToJailTile(int idx, std::string c, std::string n) 
    : Tile(idx, c, n, "GOTO_JAIL") {}

void GoToJailTile::onLanded(Player& player, GameManager& gm) {
    // Set posisi player ke indeks JailTile dan status = "JAILED"
    player.setPosision(11);
    player.setStatus("JAILED");
    player.setJailTurns(3);
}