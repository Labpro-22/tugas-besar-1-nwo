#include "models/TeleportCard.hpp"
#include "models/Player.hpp"
#include "core/GameManager.hpp"
#include "core/StateTeleportSelect.hpp"
#include <iostream>

TeleportCard::TeleportCard() : SkillCard("Pindah ke mana saja", 0, 0) {}

void TeleportCard::activate(Player& player, GameManager& gm) {
    gm.getLogger().logAction(gm.getCurrentTurnCount(), player.getUsername(), "SKILL", "Gunakan Teleport! Pilih tujuan.");
    player.setStatus("TELEPORT_SELECT"); // Memicu menu ID petak di UI
    gm.changeState(std::make_unique<StateTeleportSelect>());
}