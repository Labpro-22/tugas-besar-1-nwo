 #include "models/DemolitionCard.hpp"
 #include "models/Player.hpp"
#include "core/GameManager.hpp"
#include "core/StateDemolishSelect.hpp"
 #include <iostream>


DemolitionCard::DemolitionCard() : SkillCard("Hancurkan Properti Lawan", 0, 0) {}
void DemolitionCard::activate(Player& player, GameManager& gm) {
    gm.getLogger().logAction(gm.getCurrentTurnCount(), player.getUsername(), "SKILL", "Gunakan Demolition! Pilih gedung.");
    player.setStatus("DEMOLISH_SELECT"); 
    gm.changeState(std::make_unique<StateDemolishSelect>());
}