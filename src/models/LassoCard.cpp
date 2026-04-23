#include "models/LassoCard.hpp"
#include "models/Player.hpp"
#include "core/GameManager.hpp"
#include "core/StateLassoSelect.hpp"
#include <iostream>


LassoCard::LassoCard() : SkillCard("Tarik Lawan", 0, 0) {}
void LassoCard::activate(Player& player, GameManager& gm) {
    gm.getLogger().logAction(gm.getCurrentTurnCount(), player.getUsername(), "SKILL", "Gunakan Lasso! Pilih korban.");
    player.setStatus("LASSO_SELECT");
    gm.changeState(std::make_unique<StateLassoSelect>());
}